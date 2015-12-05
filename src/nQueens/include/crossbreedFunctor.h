#ifndef CROSSBREEDFUNCTOR_H
#define CROSSBREEDFUNCTOR_H
#include <population.h>

namespace nQueens {
class crossbreedFunctor
{
    public:
        virtual individual operator() (population & pop) = 0;
    protected:
    private:

};

}
#endif // CROSSBREEDFUNCTOR_H
