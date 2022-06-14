#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <memory/paddr.h>
enum {
  TK_NOTYPE = 256, TK_EQ, TK_NEQ, TK_NUM_10, TK_NUM_16, TK_REG,
  TK_DEREF, TK_AND, TK_NEG, TK_OR
  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   * 添加寄存器的识别模式，目前十进制常数不能为0
   */
  {"\\s+", TK_NOTYPE},    // spaces
  {"\\(", '('},         // left parentheses
  {"\\)", ')'},	        // right parentheses
  
  {"\\|\\|", TK_OR},        // 或 布尔 

  {"&&", TK_AND},        // 与 布尔 

  {"==", TK_EQ},        // equal
  {"!=", TK_NEQ},        // not equal
  
  {"\\+", '+'},         // plus
  {"\\-", '-'},         // minus
  
  {"\\*", '*'},         // muti
  {"\\/", '/'},         // divide
  
  {"\\*", TK_DEREF},    // dereference 指针解引用
  {"\\-", TK_NEG},      // 负数
  

  {"\\$[a-zA-Z\\d]+", TK_REG},       // reg name
  {"([1-9][0-9]*)", TK_NUM_10},				  // 8: 10num
  {"0[xX]([0-9a-fA-F]{1,8})", TK_NUM_16},				  // 9: 16num
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[100] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        // Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
        //    i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        // tokens[nr_token].str 的长度有限，检查是否溢出
        Assert(substr_len < 32, "tokens[nr_token].str = %d", substr_len);
        
        if(rules[i].token_type == TK_NOTYPE) break; // 忽略空格
        tokens[nr_token].type = rules[i].token_type;
        switch (rules[i].token_type) {
          case TK_NUM_10:
          case TK_NUM_16:
          case TK_REG:
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            // printf("%s\n", tokens[nr_token].str);
            break;
          // default: TODO();
        }
        ++nr_token;
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }
  printf("%s", e + position);
  return true;
}

static bool check_parentheses(int p, int q){
  if(tokens[p].type=='(' && tokens[q].type==')'){
    int t = 0;
    for(int i=p+1; i < q && t >= 0; ++i) {
      if(tokens[i].type=='(') t++;
      if(tokens[i].type==')') t--;
    }
    return (t == 0);
  }
  return false;
}

static word_t eval(int p, int q, bool* success){
  if (p > q) {
    /* Bad expression */
    *success = false;
    return -1;
  }
  else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    word_t num = 0;
    bool bl;
    switch (tokens[p].type) {
      case TK_NUM_10:
        sscanf(tokens[p].str, "%u", &num);
        break;
      case TK_NUM_16:
        sscanf(tokens[p].str, "%x", &num);
        break;
      case TK_REG:
        num = isa_reg_str2val(tokens[p].str, &bl);
        if(!bl){
          *success = false;
          Log("%s not found", tokens[p].str);
        }
        break;
      default:
        panic("error single token found");
    }
    // printf("%u\n", num);
    return num;
  }
  else if (check_parentheses(p, q) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(p + 1, q - 1, success);
  }
  else {
    int op[7], t = 0;
    memset(op, 0xff, sizeof(op));
    // 从右到左
    for(int i=q; i>=p; --i){
      switch (tokens[i].type) {
        case ')': t++;break;
        case '(': t--;break;
      }
      // 只取 一次 不在括号内的算符
      if(t == 0){
        switch (tokens[i].type){
        case TK_OR:
          if(op[0] < 0) op[0] = i;
          break;
        case TK_AND:
          if(op[1] < 0) op[1] = i;
          break;
        case TK_EQ:
        case TK_NEQ:
          if(op[2] < 0) op[2] = i;
          break;
        case '+':
        case '-':
          if(op[3] < 0) op[3] = i;
          break;
        case '*':
        case '/':
          if(op[4] < 0) op[4] = i;
          break;
        // 单目
        case TK_DEREF:
        case TK_NEG:
          if(op[5] < 0) op[5] = i;
          break;
        default:
          break;
        }
      }
      else if(t < 0) { // 括号不匹配
        *success = false;
        Log("Parentheses not match");
        return 0;
      }
       
    }
    int opc = -1;
    for(int i=0; i<(sizeof(op)/sizeof(int)); ++i){
      if(op[i]>=0){
        opc = op[i];
        printf("chc: %d %d\n",i ,opc);
        break;
      }
    }
    if(t!=0 || opc==-1) { // 括号不匹配
      *success = false;
      return 0;
    }
    // 单目运算符
    if(op[5] == opc){
      word_t val = eval(opc + 1, q, success), t;
      switch (tokens[opc].type) {
        case TK_DEREF: 
          memcpy(&t, guest_to_host(val), sizeof(word_t));
          return t;
        case TK_NEG: return (-val);
        default: panic("error unary operation found");
      }
    } 
    // 双目
    else{
      word_t val1 = eval(p, opc - 1, success);
      word_t val2 = eval(opc + 1, q, success);

      switch (tokens[opc].type) {
        case '+': return val1 + val2;
        case '-': return val1 - val2;
        case '*': return val1 * val2;
        case '/': 
          if(val2==0) return -1;
          else return val1 / val2;
        case TK_OR: return val1 || val2;
        case TK_AND: return val1 && val2;
        case TK_EQ: return val1 == val2;
        case TK_NEQ: return val1 != val2;
        default: panic("error ary operation found");
      }
    }
  }
}

word_t expr(char *e, bool *success) {
  *success = true;
  if (!make_token(e)) {
    *success = false;
    Log("Expression(%s) can't be make tokens", e);
    return 0;
  }
  for (int i = 0; i < nr_token; i ++) {
    // '*' 前只要不是数字，寄存器和')'，就是解引用
    if (i == 0 && tokens[i].type == '*') tokens[i].type = TK_DEREF;
    else if (i == 0 && tokens[i].type == '-') tokens[i].type = TK_NEG;
    else if (tokens[i].type == '*' || tokens[i].type == '-'){
      int _tag = 0;    
      switch (tokens[i - 1].type)  {
        case TK_NUM_10:
        case TK_NUM_16:
        case TK_REG:
        case ')':
          _tag = 1;
          break;
      }
      if(!_tag){
        if(tokens[i].type == '*') tokens[i].type = TK_DEREF;
        else if(tokens[i].type == '-') tokens[i].type = TK_NEG; 
      }
    }
  }
  /* TODO: Insert codes to evaluate the expression. */
  word_t ans = eval(0, nr_token - 1, success);
  if(!(*success)){
    Log("Expression(%s) can't be eval", e);
    return 0;
  }
  return ans;
}
