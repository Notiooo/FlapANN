#pragma once
#include "fann/fann.h"
#include "nodes/objects/bird/Bird.h"


/**
 * \brief Genetic algorithm used to control bird behavior
 */
class GeneticAlgorithm
{
public:

	/**
     * \brief Neural network settings
     */
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
        Unit();
        Unit(fann* ann, int index, int fitness);
        Unit(const Unit& rhs);
        Unit(Unit&& rhs) noexcept;
        Unit& operator=(const Unit& rhs);
        Unit& operator=(Unit&& rhs) noexcept;
        void performOnPredictedOutput(std::vector<fann_type> input, std::function<void(fann_type*)> perform) const;
        void mutate();
        float mutateGene(float gene);

    private:
        float mMutateRate = 0.2f;
    };

	/**
     * \brief The constructor of the genetic algorithm that initializes the Artificial Neural Network
     * \param populationSize Size of the population
     * \param topEvolvingUnits How many of the best units are used in evolution process (their genes are used to crossover)
     * \param settings Neural network settings 
     */
    GeneticAlgorithm(int populationSize, int topEvolvingUnits, NetworkSettings settings);

	/**
     * \brief Evolves a neural network by removing weak units and replacing them with a mixture of good units.
     */
    void evolve();

	/**
     * \brief Checks the population size (maximum number of units)
     * \return Size of the population
     */
    int populationSize() const;

	/**
     * \brief Creates an initial population with random weights ranging from -1 to 1.
     */
    void createPopulation();

	/**
     * \brief Returns information about which generation is currently in progress (depends on the number of evolutions)
     * \return The current generation
     */
    int currentGeneration() const;

	/**
     * \brief Returns the current population
     * \return Container with units forming the population
     */
    std::vector<Unit> population();

	/**
     * \brief Returns an individual with a given index from the entire population
     * \param index The index of the unit to get
     * \return An individual from the total population with the indicated index
     */
    Unit& at(int index);

private:

	/**
	 * \brief  Mixes two parents and returns their child.
	 * The child inherits part of the weights of one parent and part of the other parent.
	 *
	 * \param parentA One of the parents from which the weights are taken
	 * \param parentB One of the parents from which the weights are taken
	 * \return A child resulting from the mixing of the weights of both parents
	 */
	[[nodiscard]] std::unique_ptr<Unit> crossover(const Unit& parentA, const Unit& parentB) const;

	/**
	 * \brief Sorts populations by their fitness starting with the best (highest fitness score) decreasing
	 * \param population Population to be sorted
	 * \return Population sorted in descending order
	 */
	[[nodiscard]] std::vector<Unit> sortByFitness(std::vector<Unit>population) const;

	/**
     * \brief It checks if the best unit is not so hopeless already at the start that
     * it prevents or delays too much the development of the network.
     *
     * \return True if the best individual has too bad fitness score to continue
     */
    bool doesBestUnitFailed();

	/**
     * \brief Cleans up the entire population
     */
    void clearPopulation();

	/**
     * \brief Restarts the game if the best unit is too weak.
     * Then its development will be practically impossible or too slow.
     */
    void resetIfTheBestUnitIsTooWeak();

	/**
     * \brief Selects a random two units from the best and mixes their weights to form their child
     * \return A mixture of the random weights of the top random two units
     */
    std::unique_ptr<Unit> crossoverTwoRandomBestUnits();

    /**
     * \brief Selects two random units and mixes their weights to form their child
     * \return A mixture of the random weights of two random units
     */
    std::unique_ptr<Unit> crossoverTwoRandomUnits();

    /**
     * \brief Selects two units from the best and mixes their weights to form their child
     * \return A mixture of the random weights of the top two units
     */
    std::unique_ptr<Unit> crossoverTwoBestUnits();

	/**
	 * \brief Reassigns the indexes inside population starting from zero to the end
	 */
	void reassignIndexes();

	/**
     * \brief Assigns a new population by reassigning indexes
     * \param sortedPopulation Population to be assigned
     */
    void reassignPopulation(std::vector<Unit> sortedPopulation);

	/**
     * \brief All weak birds are swapped for crossover between the best units and a few random ones
     * \param sortedPopulationByFitness Population sorted by fitness score in descending order
     * \return Returns a population where the top units stay the same and the weaker
     * ones are swapped out with a mix of the better ones
     */
    std::vector<Unit> replaceWeakBirdsWithCrossovers(std::vector<Unit>&& sortedPopulationByFitness);

	/**
     * \brief Evolves the current population by replacing weak birds with a mix of better ones
     */
    void evolveWeakUnits();

	/**
     * \brief Returns the best individuals from the current population
     * \return Best units from the current population
     */
    std::vector<Unit> bestUnits();

private:
    /**
     * The minimum acceptable fitness score that should
     * appear for the top first generation unit
     */
    static inline float minimumFitnessScore = 2.f;

    /** Size of the population */
    int mSizeOfPopulation;

    /** Size of how much of the population might consider to be top units */
    int mTopUnits;

    /** Layers of the Artificial Neural Network */
    std::vector<unsigned> mLayers;

    /** An entire population consisting of units */
    std::vector<Unit> mPopulation;

    /** Number indicating the current generation iteration */
    int mCurrentGeneration;
};
