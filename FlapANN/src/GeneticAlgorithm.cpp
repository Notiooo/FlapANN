#include "pch.h"
#include "GeneticAlgorithm.h"

GeneticAlgorithm::~GeneticAlgorithm()
{
    clearPopulation();
}

GeneticAlgorithm::Unit::~Unit()
{
    fann_destroy(ann);
}

void GeneticAlgorithm::Unit::performOnPredictedOutput(std::vector<fann_type> input, std::function<void(fann_type*)> perform) const
{
    perform(fann_run(ann, input.data()));
}

void GeneticAlgorithm::Unit::mutate()
{
    for(int i = 0; i < ann->total_connections; ++i)
    {
        ann->weights[i] = mutateGene(ann->weights[i]);
    }
    for (int i = 0; i < ann->total_connections; ++i)
    {
        ann->connections[i]->activation_steepness = mutateGene(ann->connections[i]->activation_steepness);
    }
}

float GeneticAlgorithm::Unit::mutateGene(float gene)
{
    auto randomBetweenZeroAndOne = []()
    {
        return (static_cast<float>(std::rand()) / (RAND_MAX));
    };
    auto random = randomBetweenZeroAndOne();
    if(random < mMutateRate)
    {
        const auto mutateFactor = 1.f + ((randomBetweenZeroAndOne() - 0.5f) * 3.f + (randomBetweenZeroAndOne() - 0.5f));
        gene *= mutateFactor;
    }
    return gene;
}

GeneticAlgorithm::GeneticAlgorithm(int population, int topEvolvingUnits, NetworkSettings settings)
    : mMaxUnits(population)
    , mTopUnits(topEvolvingUnits)
    , mCurrentGeneration(0)
{
    mLayers.push_back(settings.mInputNeurons);
    mLayers.insert(mLayers.begin()+1, settings.mNeuronsPerHiddenLayer.begin(), settings.mNeuronsPerHiddenLayer.end());
    mLayers.push_back(settings.mOutputNeurons);
}

bool GeneticAlgorithm::doesBestUnitFailed()
{
    return bestUnits().at(0).fitness < 0.f;
}

std::vector<GeneticAlgorithm::Unit> GeneticAlgorithm::sortByIndex(std::vector<Unit> population) const
{
    std::sort(population.begin(), population.end(), [](const Unit& a, const Unit& b)
    {
        return a.index < b.index;
    });

    return population;
}

void GeneticAlgorithm::clearPopulation()
{
    mPopulation.clear();
}

void GeneticAlgorithm::checkBestUnitCorrectness()
{
    if(doesBestUnitFailed())
    {
        clearPopulation();
        createPopulation();
    }
}

std::unique_ptr<GeneticAlgorithm::Unit> GeneticAlgorithm::crossoverTwoRandomBestUnits()
{
	auto& bestUnits = this->bestUnits();
	std::vector<Unit> randomBests;
	std::sample(bestUnits.begin(), bestUnits.end(), std::back_inserter(randomBests), 2,
	            std::mt19937(std::random_device()()));
	return crossover(randomBests.at(0), randomBests.at(1));
}

std::unique_ptr<GeneticAlgorithm::Unit> GeneticAlgorithm::crossoverTwoRandomUnits()
{
	auto population = this->population();
	return std::make_unique<Unit>(*(population.begin() + (std::rand() % population.size())));
}

std::unique_ptr<GeneticAlgorithm::Unit> GeneticAlgorithm::crossoverTwoBestUnits()
{
	auto bestUnits = this->bestUnits();
	return crossover(bestUnits.at(0), bestUnits.at(1));
}

void GeneticAlgorithm::reassignPopulation(std::vector<GeneticAlgorithm::Unit> sortedPopulation)
{
	mPopulation = sortedPopulation;
	int iterator = 0;
	for(auto& unit : mPopulation)
	{
		unit.index = iterator++;
	}
}

std::vector<GeneticAlgorithm::Unit> GeneticAlgorithm::replaceWeakBirdsWithCrossovers(std::vector<GeneticAlgorithm::Unit>&& sortedPopulationByFitness)
{
	const auto firstWeakUnitIndex = mTopUnits;
	const auto& populationSizeWithoutTopUnits = mPopulation.size() - mTopUnits;

	for(int i = 0; i < populationSizeWithoutTopUnits; ++i)
	{
		auto offspring = std::unique_ptr<Unit>();

		if(i == 0)
		{
			offspring = crossoverTwoBestUnits();
		}
		else if (i < populationSizeWithoutTopUnits - 2)
		{
			offspring = crossoverTwoRandomBestUnits();
		}
		else
		{
			offspring = crossoverTwoRandomUnits();
		}

		offspring->mutate();
		sortedPopulationByFitness[firstWeakUnitIndex + i] = *offspring.release();
	}
    return sortedPopulationByFitness;
}

void GeneticAlgorithm::evolveOtherThanBestUnits()
{
	reassignPopulation(replaceWeakBirdsWithCrossovers(sortByFitness(mPopulation)));
	++mCurrentGeneration;
}

void GeneticAlgorithm::evolve()
{
    checkBestUnitCorrectness();

    evolveOtherThanBestUnits();
}

std::vector<GeneticAlgorithm::Unit> GeneticAlgorithm::sortByFitness(std::vector<GeneticAlgorithm::Unit> population) const
{
    std::sort(population.begin(), population.end(), [](const Unit& a, const Unit& b)
    {
        return a.fitness > b.fitness;
    });

    return population;
}

std::vector<GeneticAlgorithm::Unit> GeneticAlgorithm::bestUnits()
{
    auto sorted = sortByFitness(mPopulation);

    return std::vector(sorted.begin(), sorted.begin() + mTopUnits);
}

int GeneticAlgorithm::maxUnits() const
{
    return mMaxUnits;
}

void GeneticAlgorithm::createPopulation()
{
    for (int i = 0; i < maxUnits(); ++i)
    {
        auto ann = fann_create_standard_array(mLayers.size(), mLayers.data());
        Unit unit = {ann, i, 0};
        mPopulation.push_back(unit);
    }
    for(auto& unit : mPopulation)
    {
        fann_randomize_weights(unit.ann, -1.f, 1.f);
    }
}

int GeneticAlgorithm::currentGeneration() const
{
    return mCurrentGeneration;
}

std::vector<GeneticAlgorithm::Unit> GeneticAlgorithm::population()
{
    return mPopulation;
}

GeneticAlgorithm::Unit& GeneticAlgorithm::at(int index)
{
    return mPopulation.at(index);
}

std::unique_ptr<GeneticAlgorithm::Unit> GeneticAlgorithm::crossover(const Unit& parentA, const Unit& parentB) const
{
    static std::random_device rd; 
    static std::mt19937 gen(rd());
    const std::uniform_int_distribution<> distr(0, parentA.ann->total_connections-1);
    const std::bernoulli_distribution trueOrFalse;

    auto cutPoint = distr(gen);
    for (int i = cutPoint; i < parentA.ann->total_connections; ++i)
    {
        auto biasFromA = parentA.ann->weights[i];
        parentA.ann->weights[i] = parentB.ann->weights[i];
        parentB.ann->weights[i] = biasFromA;
    }

    return (trueOrFalse(gen) ? std::make_unique<Unit>(parentA) : std::make_unique<Unit>(parentB));
}

