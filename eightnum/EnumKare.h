#pragma once
#include <vector>
#include <sstream>

#include "defines.h"
#include "EnumMap.h"
#include "utils.h"

using namespace std;

struct SGenome
{
	vector<int> vecBits;

	double		dFitness;


	SGenome() :dFitness(0) {}

	SGenome(const int num_bits) :dFitness(0)
	{
		//create a random bit string
		for (int i = 0; i<num_bits; ++i)
		{
			vecBits.push_back(RandInt(0, 1));
		}
	}
};

class EnumKare
{
public:
	EnumKare();
	~EnumKare();
	EnumKare(double cross_rat,
		double mut_rat,
		int    pop_size,
		int	num_bits,
		int    gene_len) :m_dCrossoverRate(cross_rat),
		m_dMutationRate(mut_rat),
		m_iPopSize(pop_size),
		m_iChromoLength(num_bits),
		m_dTotalFitnessScore(0.0),
		m_iGeneration(0),
		m_iGeneLength(gene_len),
		m_bBusy(false)
	{
		CreateStartPopulation();
	}
	void  Run(HWND hwnd);
	void  Epoch();
	void  Render(int cxClient, int cyClient, HDC surface);
	//访问用的方法
	int	  Generation() { return m_iGeneration;}
	int	  GetFittest() { return m_iFittestGenome; }
	bool  Started() { return m_bBusy; }
	void  Stop() { m_bBusy = false; }


private:
	//基因组群
	vector<SGenome> m_vecGenomes;
	//群体大小
	int				m_iPopSize;
	double			m_dCrossoverRate;
	double			m_dMutationRate;
	//每个染色体有多少bits
	int				m_iChromoLength;
	//每个基因有多少bits
	int				m_iGeneLength;
	int				m_iFittestGenome;
	double			m_dBestFitnessScore;
	double			m_dTotalFitnessScore;
	int				m_iGeneration;
	int				m_ishortsteps;
	//为map类创建一个实例
	EnumMap			m_KareMap;
	//创建另一个对象，用于保存和显示最佳路径
	EnumMap			m_KareBrain;
	//检测是否仍在进行中
	bool			m_bBusy;
	void			Mutate(vector<int> &vecBits);
	void			Crossover(const vector<int> &mun,
		const vector<int> &dad,
		vector<int> &baby1,
		vector<int> &baby2);

	SGenome&		RouletteWheelSelection();
	//用新的适应性分数来更新基因组原有的适应性分数并计算最高适应性分数
	void			UpdataFitnessScores();
	//把一个位向量译为一个方向的向量
	vector<int>		Decode(const vector<int> &bits);
	//把一个位向量变换为十进制数，用于译码  
	int				BinToInt(const vector<int> &v);
	//创建一个随机的二进制位串的初始群体
	void			CreateStartPopulation();


};

