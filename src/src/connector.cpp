#include "connector.h"
#include "execute.h"
using namespace std;



bool andOp::executeStatement()
{
   execute temp;
   
   if (temp.executeStatement(this->left))   
   {
       if(temp.executeStatement(this->right))
       {
           return true;
       }
   }
   return false;
}

bool orOp::executeStatement()
{
    execute temp;
    
    if (!(temp.executeStatement(this->left)))
    {
        if(temp.executeStatement(this->right))
        {
            return true;
        }
    }
    return false;
}


bool semicolon::executeStatement()
{
    execute temp;
    
    temp.executeStatement(this->left);
    if(temp.executeStatement(this->right))
    {
        return true;
    }
    
    return false;
}




