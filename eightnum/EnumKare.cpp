#include "EnumKare.h"

EnumKare::EnumKare()
{
}

EnumKare::~EnumKare()
{
}

SGenome& EnumKare::RouletteWheelSelection()
{
	double fSlice = RandFloat()*m_dTotalFitnessScore;
	double cfTotal = 0;
	int		SelectedGenome = 0;
	int i;
	for (i = 0; i < m_iPopSize; i++)
	{
		cfTotal += m_vecGenomes[i].dFitness;
		if (cfTotal > fSlice)
		{
			SelectedGenome = i;
			break;
		}
	}
	return m_vecGenomes[SelectedGenome];
}

void EnumKare::Mutate(vector<int> &vecBits)
{
	for (int curBit = 0; curBit < vecBits.size(); curBit++)
	{
		if ( RandFloat() < m_dMutationRate)
		{
			vecBits[curBit] = !vecBits[curBit];
		}
	}
}

void EnumKare::Crossover(const vector<int> &mum,
	const vector<int> &dad,
	vector<int> &baby1,
	vector<int> &baby2)
{
	//由随机概率决定是否杂交,若两亲本相同也不杂交
	//不杂交则直接复制亲本
	if (RandFloat() > m_dCrossoverRate || mum == dad)
	{
		baby1 = dad;
		baby2 = mum;
		return;
	}
	int cp = RandInt(0, m_iChromoLength - 1);
	int i;
	for ( i = 0; i < cp; i++)
	{
		baby1.push_back(mum[i]);
		baby2.push_back(dad[i]);
	}
	for ( i = cp; i < mum.size(); i++)
	{
		baby1.push_back(dad[i]);
		baby2.push_back(mum[i]);
	}
}

void EnumKare::Run(HWND hwnd)
{
	CreateStartPopulation();

	m_bBusy = true;
	//int i;
	//for (i = 0; i < 500; i++)
	//{
	//	Epoch();
	//	if (m_bBusy == false)
	//	{
	//		return;
	//	}
	//}
	//i = 0;
}

void EnumKare::CreateStartPopulation()
{
	//清场
	m_vecGenomes.clear();
	int i;
	for ( i = 0; i < m_iPopSize; i++)
	{
		m_vecGenomes.push_back(SGenome(m_iChromoLength));

	}
	//reset
	m_iGeneration = 0;
	m_iFittestGenome = 0;
	m_dBestFitnessScore = 0;
	m_dTotalFitnessScore = 0;
}

void EnumKare::Epoch()
{
	UpdataFitnessScores();

	int NewBabies = 0;

	vector<SGenome> vecBabyGenomes;

	while (NewBabies < m_iPopSize)
	{
		SGenome mum = RouletteWheelSelection();
		SGenome dad = RouletteWheelSelection();

		SGenome baby1, baby2;
		Crossover(mum.vecBits, dad.vecBits, baby1.vecBits, baby2.vecBits);

		Mutate(baby1.vecBits);
		Mutate(baby2.vecBits);

		vecBabyGenomes.push_back(baby1);
		vecBabyGenomes.push_back(baby2);

		NewBabies += 2;

	}

	m_vecGenomes = vecBabyGenomes;

	++m_iGeneration;
}

void EnumKare::Render(int cxClient, int cyClient, HDC surface)
{
	m_KareBrain.Render(cxClient, cyClient, surface);
	string s;
	if (m_dBestFitnessScore > 1)
	{
		s = "迭代次数: " + itos(m_iGeneration) + ", 最少步数:" + itos((int)500 / (m_dBestFitnessScore - 1));
	}
	else {
		s = "迭代次数: " + itos(m_iGeneration) + ", 最高分:" + itos((int)1 / (m_dBestFitnessScore));
	}
	TextOut(surface, 5, 0, s.c_str(), s.size());
}

void EnumKare::UpdataFitnessScores()
{
	m_iFittestGenome = 0;
	m_dBestFitnessScore = 0;
	m_dTotalFitnessScore = 0;

	
	for (int i = 0; i < m_iPopSize; ++i)
	{
		EnumMap TempMemory;
		vector<int> vecDirections = Decode(m_vecGenomes[i].vecBits);
		m_vecGenomes[i].dFitness =  m_KareMap.TestRoute(vecDirections, TempMemory);
		//record the bestscore
		if (m_dBestFitnessScore < m_vecGenomes[i].dFitness)
		{
			m_dBestFitnessScore = m_vecGenomes[i].dFitness;
			m_iFittestGenome = i;
			m_KareBrain = TempMemory;
			if (m_dBestFitnessScore == 1)
			{
				m_bBusy = false;
			}
		}
		//sum all score
		m_dTotalFitnessScore += m_vecGenomes[i].dFitness;
	}

}

//解码一组二进制到一个方向向量
//0上 1 下 2 左 3 右
vector<int> EnumKare::Decode(const vector<int> &bits)
{
	vector<int> directions;
	int  gene = 0;
	for (; gene < bits.size(); gene+= m_iGeneLength)
	{
		vector<int> ThisGene;
		int bit = 0;
		for (; bit < m_iGeneLength; ++bit)
		{
			ThisGene.push_back(bits[gene + bit]);
		}
		//转换为十进制并加入到方向向量中
		directions.push_back(BinToInt(ThisGene));
	}

	return directions;
}

int EnumKare::BinToInt(const vector<int> &vec)
{
	int val = 0;
	int multiplier = 1;
	int cBit = vec.size();
	for (; cBit > 0 ; cBit--)
	{
		val += vec[cBit - 1] * multiplier;

		multiplier *= 2;
	}

	return val;
}
