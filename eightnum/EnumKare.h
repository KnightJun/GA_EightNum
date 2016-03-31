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
	//�����õķ���
	int	  Generation() { return m_iGeneration;}
	int	  GetFittest() { return m_iFittestGenome; }
	bool  Started() { return m_bBusy; }
	void  Stop() { m_bBusy = false; }


private:
	//������Ⱥ
	vector<SGenome> m_vecGenomes;
	//Ⱥ���С
	int				m_iPopSize;
	double			m_dCrossoverRate;
	double			m_dMutationRate;
	//ÿ��Ⱦɫ���ж���bits
	int				m_iChromoLength;
	//ÿ�������ж���bits
	int				m_iGeneLength;
	int				m_iFittestGenome;
	double			m_dBestFitnessScore;
	double			m_dTotalFitnessScore;
	int				m_iGeneration;
	int				m_ishortsteps;
	//Ϊmap�ഴ��һ��ʵ��
	EnumMap			m_KareMap;
	//������һ���������ڱ������ʾ���·��
	EnumMap			m_KareBrain;
	//����Ƿ����ڽ�����
	bool			m_bBusy;
	void			Mutate(vector<int> &vecBits);
	void			Crossover(const vector<int> &mun,
		const vector<int> &dad,
		vector<int> &baby1,
		vector<int> &baby2);

	SGenome&		RouletteWheelSelection();
	//���µ���Ӧ�Է��������»�����ԭ�е���Ӧ�Է��������������Ӧ�Է���
	void			UpdataFitnessScores();
	//��һ��λ������Ϊһ�����������
	vector<int>		Decode(const vector<int> &bits);
	//��һ��λ�����任Ϊʮ����������������  
	int				BinToInt(const vector<int> &v);
	//����һ������Ķ�����λ���ĳ�ʼȺ��
	void			CreateStartPopulation();


};

