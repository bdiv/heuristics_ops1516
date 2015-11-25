#ifndef VARIATION_H
#define VARIATION_H


class variation
{
    public:
        static void mutation(individual & i);
        static void crossbreed(population & pop);
        virtual ~variation();
    protected:
    private:
        variation();
};

#endif // VARIATION_H
