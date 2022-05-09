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
        int fitness;

        Unit()
            : ann(nullptr), index(0), fitness(0) {}
        Unit(fann* ann, int index, int fitness)
            : ann(ann), index(index), fitness(fitness) {}

        Unit(const Unit& rhs) = default;
        Unit(Unit&& rhs) = default;
        Unit& operator=(const Unit& rhs) = default;

        void performOnPredictedOutput(std::vector<fann_type> input, std::function<void(fann_type*)> perform) const;
        void mutate();
        float mutateGene(float gene);

    private:
        float mMutateRate = 0.2f;
    };

    enum CrossoverStage
    {
        PickTopTwoBests,
        PickRandomBests,
        PickRandomly
    };

    GeneticAlgorithm(int population, int topEvolvingUnits, NetworkSettings settings);
    void evolve();
    int maxUnits() const;
    void createPopulation();
    int currentGeneration();
    std::vector<Unit> population();
    Unit& at(int index);

private:
    std::unique_ptr<Unit> crossover(const Unit& parentA, const Unit& parentB);
    std::vector<Unit> sortByFitness(std::vector<Unit>population);
    std::vector<Unit> sortByIndex(std::vector<Unit> population) const;
    bool bestUnitFailed();
    void clearPopulation();
    void checkBestUnitCorrectness();
    std::unique_ptr<Unit> performCrossover(CrossoverStage crossoverStage);
    std::vector<Unit> bestUnits();

private:
    int mMaxUnits;
    int mTopUnits;
    std::vector<unsigned> mLayers;
    std::vector<Unit> mPopulation;
    int mCurrentGeneration;
};
