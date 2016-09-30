// Object Oriented Calculator Program
// Started: 29/09/16

#include <iostream>   // standard input/output
#include <cmath>
#include <map>        // for dictionary functionality
#include "infix.cpp"  // contains the Token struct and the apply_op() function

using namespace std;

/**
    Our Calculator object which will do pretty much everything.
*/
class Calculator {
public:
    Calculator() :prev_result{0} {}
    void get_statement();
    long evaluate( string expr, bool &error );
private:
    /*    MEMORY     */
    double prev_result;
    map<string,double> variables;
    /******************/
};

// TODO: calculator currently only returns integer values
/**
    Accepts an equation in INFIX notation and returns the answer
    as a long.
*/
long Calculator::evaluate( string expr, bool &error ) {

    stack<Token> s;
    stack<long>  operands;
    int i;
    Token token;
    bool used_prev_result = false;
    bool found_first_num = false;

    error = false;
    i = 0;
    while( token.next_token( expr, i, error) && !error ) {
        switch(token.type) {
            case Token::NUMBER:
                operands.push(token.val);
                found_first_num = true;
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
            case Token::KEYWORD: // TODO
                break;
            case Token::VAR: // TODO
                break;
            default: // arithmetic operators
                // we have hit an operator so check if we have a left-hand
                // number to use already. If not, use prev result value
                if( !found_first_num && !used_prev_result ) {
                    operands.push(this -> prev_result);
                    used_prev_result = true;
                }
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
    this->prev_result = operands.top();
    return this->prev_result;
}

/**
    Handles the prompting and recieving of an equation from the user.
*/
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

/**
    Create our calculator object and run its 'driver' function.
*/
int main () {
    Calculator imba;
    imba.get_statement();
    return 0;
}
