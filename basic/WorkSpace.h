/*
 * WorkSpace.h
 *
 * Last Updated: 2004 Nov 18th  by inamura
 * Last Updated: 2008-07-17 by inamura
 *
 * 2008-07
 *	added CalcKLDivergenceMatrix and CalcHellingerMatrix instead of CalcDistanceMatrix
 */

#ifndef __WORKSPACE_H__
#define __WORKSPACE_H__

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "mimesis.h"
#include "MotionDB.h"
#include "HTKUnit.h"
#include "PsymbolSpace.h"

typedef enum
{
  KL_DIVERGENCE,
  HELLINGER,
  DISTANCE_UNKNOWN
}
DistanceType;


class WorkSpace
{
  string			work_dir;	// ��ƃf�B���N�g����
  ifstream			fin;		// �X�N���v�g�t�@�C���ǂݍ��ݗp
  vector<MotionDB *>		motion_db;	// MotionDB:HMM�w�K�p(�w�K�t�F�[�Y��͕s�v)
  vector<HTKUnit *>		htk_units;	// HTKUnit : for interface with HTK, subclass of JHMM
  vector<vector <double > >	matrix_KLD;	// ��Ԃ��\�����邽�߂�KL���� (matrix)
  vector<vector <double > >	matrix_HD;	// ��Ԃ��\�����邽�߂�Hellinger���� (matrix)
  vector<string>		label;		// recog_units, motion_db�̍s����label   TODO : ������ label �Ǘ����Ă���̂͂�������
  PsymbolSpace			*space;		// ���n�V���{�����
  int				span;		// �s�������n�V���{����ԂŔF������Ƃ��̃^�C���X�p��
  int				step;		// �^�C���X�e�b�v
  DistanceType			d_type;

 public: // for test
  Behavior* beh_buf;
  // ���������s����ێ����Ă����o�b�t�@�[
  // �s�������n�V���{����ԂŐ��������肻���
  // ����o�����肷��ɂꐏ��beh_buf��������ς���

  WorkSpace();
  WorkSpace(DistanceType type);
  ~WorkSpace();
  
  //  int		LoadLearningScriptFile	(const char *fname);
  int		AddMotionDB		(MotionDB* mdb);
  MotionDB	*NthMotionDB		(int nth);
  int		GetNumOfMotionDB	(void);
  int		AddHTKUnit		(HTKUnit* rec);
  HTKUnit	*GetNthHTKUnit		(int nth);
  int		GetNumOfHTKUnits	(void);
  int		AddPsymbol		(JHMM *new_psymbol);
  //int		SetPsymbols		(void);
  int		SetWorkDir		(const char *dir_name);
  string	GetWorkDir		(void);
  int		LoadMotionDB		(const char *fname);
  int		LoadVersion1		(void);
  int		LoadVersion2		(void);
  int		LoadVersion3		(const char *arg);
  int		LoadScaled		(const char* fname);
  int		FileOut			(const char* fname);
  int		ExecLearning		(void);

  //vector<double> CalcLikelihoodVector	(Behavior *beh);
  int		CalcLikelihoodVector	(Behavior *beh, vector<double> &like_vector);
  int		BeforeRecognize		(void);
  int		CreateWorkDir		(void);
  int		NumOfHTKUnits		(void);

  //vector<vector <double > >	CalcDistanceMatrix(void);
  int		CalcKLDivergenceMatrix	(void);
  int		CalcHellingerMatrix	(void);
  int		SetHTKUnitsFromMotionDB	(void);
  int		LoadHTKUnits		(const char *fname);
  //int		SetDisVector		(void);	//deleted on 2008-07-17
  int		SymmentrizeDisVector	(void);
  int		DistanceFileOut		(const char *fname);
  int		DistanceLoad		(const char *fname);

  int		AddLabel		(const char *name);
  int		AddLabel		(string name);
  int		SetLabelFromMotionDB	(void);
  int		SetLabelFromHTKUnit	(void);
  int		GetKey			(const char *name);
  int		GetKey			(string &name);
  string	& GetNthLabel		(int nth);
  
  int		SpaceCreate				(int dim, DistanceType d_type);
  Behavior*	BehaviorGenerateFromSinglePoint		(vector<double>& pos);
  int		BehGeneFromTransition			(vector<double>& pos);
  int		BehBufFileOut				(const char* fname);
  vector<double>	GetCoordinate			(const char* name);
  //vector<double>	CalcDistanceOfInputBehavior	(Behavior* beh);
  //vector<double>	CalcDistanceOfOnlineBehavior	(Behavior *beh);
  //vector<double>	CalcHellingerDistanceOfOnlineBehavior(Behavior *beh);
  int			CalcDistanceOfInputBehavior	(Behavior* beh, vector<double> &distance);
  int			CalcDistanceOfOnlineBehavior	(Behavior *beh, vector<double> &distance);
  int			CalcHellingerDistanceOfOnlineBehavior(Behavior *beh, vector<double> &distance);

  PsymbolSpace* GetPsymbolSpace		(void);
  int		SpaceFileOut		(const char* fname);
  int		SpaceLoad		(const char* fname);
  int		GetStepTime		(void);
  Behavior*	GetLastBehavior		(void);
  Behavior	*GetBehBuf		(void);
  int		Verify			(void);
};


#endif /* __WORKSPACE_H__ */
