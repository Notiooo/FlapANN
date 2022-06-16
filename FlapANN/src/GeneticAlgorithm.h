#pragma once
#include "fann/fann.h"
#include "nodes/objects/bird/Bird.h"

class GeneticAlgorithm
{
public:
    ~GeneticAlgorithm();

    struct NetworkSettings
    {
        unsigned mInputNeurons;
        std::vector<int> mNeuronsPerHiddenLayer;   
        unsigned mOutputNeurons;
    };

    struct Unit
    {
        fann* ann;
        int index;
        float fitness;

        ~Unit();

        Unit()
            : ann(nullptr), index(0), fitness(0) {}
        Unit(fann* ann, int index, int fitness)
            : ann(ann), index(index), fitness(fitness) {}

        Unit(const Unit& rhs)
            : ann(fann_copy(rhs.ann)), index(rhs.index), fitness(rhs.fitness) {}
        Unit(Unit&& rhs) noexcept
            : ann(rhs.ann), index(rhs.index), fitness(rhs.fitness)
        {
            rhs.ann = nullptr;
        }

        Unit& operator=(const Unit& rhs)
        {
            if (this != &rhs)
            {
                ann = fann_copy(rhs.ann);
                index = rhs.index;
                fitness = rhs.fitness;
                mMutateRate = rhs.mMutateRate;
            }
            return *this;
        }

        Unit& operator=(Unit&& rhs) noexcept
        {
            ann = rhs.ann;
            index = rhs.index;
            fitness = rhs.fitness;
            mMutateRate = rhs.mMutateRate;
            rhs.ann = nullptr;
            return *this;
        }

        void performOnPredictedOutput(std::vector<fann_type> input, std::function<void(fann_type*)> perform) const;
        void mutate();
        float mutateGene(float gene);

    private:
        float mMutateRate = 0.2f;
    };

    enum CrossoverStage
    {
        PickTopTwoBestUnits,
        PickRandomBestUnits,
        PickRandomUnit
    };

    GeneticAlgorithm(int population, int topEvolvingUnits, NetworkSettings settings);
    void evolve();
    int maxUnits() const;
    void createPopulation();
    int currentGeneration() const;
    std::vector<Unit> population();
    Unit& at(int index);

private:
    std::unique_ptr<Unit> crossover(const Unit& parentA, const Unit& parentB) const;
    std::vector<Unit> sortByFitness(std::vector<Unit>population) const;
    std::vector<Unit> sortByIndex(std::vector<Unit> population) const;
    bool doesBestUnitFailed();
    void clearPopulation();
    void checkBestUnitCorrectness();
    std::unique_ptr<Unit> crossoverTwoRandomBestUnits();
    std::unique_ptr<Unit> crossoverTwoRandomUnits();
    std::unique_ptr<Unit> crossoverTwoBestUnits();
    void reassignPopulation(std::vector<GeneticAlgorithm::Unit> sortedPopulation);
    std::vector<GeneticAlgorithm::Unit> replaceWeakBirdsWithCrossovers(std::vector<GeneticAlgorithm::Unit>&& sortedPopulationByFitness);
    void evolveOtherThanBestUnits();
    std::vector<Unit> bestUnits();

private:
    int mMaxUnits;
    int mTopUnits;
    std::vector<unsigned> mLayers;
    std::vector<Unit> mPopulation;
    int mCurrentGeneration;
};
