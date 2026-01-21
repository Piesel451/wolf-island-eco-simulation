#pragma once

struct SimulationConfig
{
	int rows = 10;
	int cols = 10;

	int rabbitCount = 2;
	float rabbitStartingEnergy = 100.f;
	float rabbitMaxEnergy = 100.f;
	int rabbitReproduceProb = 15;
	float rabbitEnergyLoss = 5.0f;

	int maleWolvesCount = 1;
	int femaleWolvesCount = 1;
	float wolfStartingEnergy = 100.0f;
	float wolfMaxEnergy = 100.f;
	float wolfEnergyGain = 10.0f;
	float wolfEnergyLoss = 5.0f;

	float simulationSpeed = 0.04f;
};