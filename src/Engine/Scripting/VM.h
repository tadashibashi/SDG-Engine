/* =============================================================================
 * VM
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <stack>

static const unsigned MAX_STACK = 512;

namespace SDG
{
    class VM {
    public:
        VM();
        ~VM();
        void Push(unsigned val);
        unsigned Pop();
    private:
        std::stack<unsigned> stack_;
    };

}



