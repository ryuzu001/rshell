#include "connector.h"
using namespace std;


bool andOp::executeStatement()
{
   
    
    if (this->left.executeStatement() && this->right.execute::executeStatement())
    {
        return;
    }
}