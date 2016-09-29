// Object Oriented Calculator Program
// Started: 29/09/16

#include <iostream>
#include "infix.cpp"

using namespace std;

class Calculator {
public:
    void get_statement();
    long evaluate( string expr, bool &error );
    long prev_result;
};

long Calculator::evaluate( string expr, bool &error ) {

    stack<Token> s;
    stack<long>  operands;
    int i;
    Token token;

    error = false;
    i = 0;
    while( token.next_token( expr, i, error) && !error ) {
        switch(token.type) {
            case Token::NUMBER:
                operands.push(token.val);
                break;
            case Token::LEFT_PAREN:
                s.push(token);
                break;
            case Token::RIGHT_PAREN:
                while(!(error = s.empty()) && s.top().type != Token::LEFT_PAREN) {
                    if((error = !apply_op(operands, s.top()))) {
                      break;
                    }
                    s.pop();
                }
                if( !error ) {
                  s.pop();
                }
                break;
            default:          // arithmetic operators
                while( !error && !s.empty() &&
                       (token.get_priority() < s.top().get_priority() ||
                        token.get_priority() == s.top().get_priority() &&
                        token.is_left_assoc()))
                {
                    error = !apply_op(operands, s.top());
                    s.pop();
                }
                if( !error ) {
                  s.push(token);
                }
        }
        if( error )  { break; }
    }
    while( !error && !s.empty() ) {
        error = !apply_op(operands, s.top() );
        s.pop();
    }
    error |= (operands.size() != 1 );
    if ( error ) { return 0; }
    return operands.top();
}

void Calculator::get_statement() {
    int result;
    string expr;
    bool error;

    cout << "Enter your equation" << endl;
    getline(cin, expr);
    while( !cin.eof()) {
        result = evaluate(expr, error); // we don't need Calculator::evaluate because we are already in scope
        if( error ) {
            cout << "Invalid expression" << '\n';
        } else {
            cout << " = " << result << '\n';
        }
        cout << "Enter your equation" << endl;
        getline(cin, expr);
    }

}
int main () {
    Calculator imba;
    imba.get_statement();
    return 0;
}
