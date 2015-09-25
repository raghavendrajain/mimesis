/*
 * Mds.h
 * multiple dimension 
 * Modified : Tetsunari Inamura on 2003 Nov 21
 * Modified : Tetsunari Inamura on 2004 Jun 1st
 */

#ifndef __MDS_H__
#define __MDS_H__

#include <fstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <math.h>

//#include "Common.h"

using namespace std;


// �ɤ�������¤�ˤ��뤫�����ȹͤ��ͤС�����
// ����Ū�ʵ�ǽ�ϵ�Υ�ǡ�����Ϳ��, ��ɸ���֤���
class MDS
{
  vector<vector<double> > original_data;	// ���ꥸ�ʥ�ε�Υ�ǡ���
  vector<vector<double> > target_distance;	// ��ɸ�Ȥʤ��Υ(��2��)
  vector<int> i_num;
  vector<int> j_num;
  double normalize;

 protected:
  int dimension;
  vector<vector<double> > coordinate;		// ���ϥ���ܥ�ζ��־�κ�ɸ
  vector<vector<double> > distance;

 public:
  MDS();
  MDS(int dim);  
  MDS(const MDS& another);
  ~MDS();

  int		Reset		(void);
  int		SetDimension	(int dim);
  int		SetData		(const char* fname);
  int		SetData		(double **data, int size);
  int		AddData		(double *data, int size);
  int		AddData		(vector<double> vec);
  int		Show		(void);
  int		GetNumOfData	(void);
  int		GetDimension	(void);
  double GetNormalize();
  void SetNormalize(double normal);
  
  void InitCoordinate();
  void DataStandarize();
  void CalcDistance();
  void Junjo();
  double CalcEvalValue();
  void UpdateCoordinate();
  void UpdateCoordinateEach(int num_index, int dim_index);
  void TargetDistanceUpdate();
  
  int			CoordinateFromDistanceData		(vector<double> &dis_data, vector<double> &result);
  vector<double>	InputDataConvertRealDistance		(vector<double> &input);
  double		CalcEvalValueOfInputData		(vector<double> &real_distance, vector<double> &tmp_cord);
  void			UpdateCoordinateFromInputData		(vector<double> &tmp_dist, vector<double> &tmp_cord);

  vector<double>	CalcDistanceFromOneCoordinate		(vector<double> &tmp_cord);
  vector<double>	GetRatio				(vector<double> &tmp_cord);
  vector<double>	GetNthCoordinate			(int nth);

  int FileOut(const char* fname);
  int FileOut(ofstream& fout);
  int Load(const char* fname);
  int Load(ifstream& fin);
  void GetCoordinate( vector<vector<double> >&  vecCoordinate);
};


#endif /* __MDS_H__ */
