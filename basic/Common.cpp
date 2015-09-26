#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <fstream>
#include <iostream>
#include <string.h>
#define __COMMON_CPP__
#include "mimesis.h"


// " ./ " ����ϤޤäƤ������ $PWD ����֤��롥
// "/home/xxxxx" �ʤɤΤ褦�����Хѥ��ˤʤäƤ���Ф��Τޤޡ�
// "../" ����ϤޤäƤ���С�PWD ��������Хѥ��Ȥ��Ƶ��롥
// �嵭�Τ�����Ǥ�ʤ���С�ۣ��ʥѥ��Ǥ����Ƚ�Ǥ��ơ��Ķ��ѿ��� MIMESIS_TOP_DIR ���䴰����
// memo : 2005 May 23 : kojo���δĶ��� getenv("PWD")�� c:/cygwin/home/...�Ȥʤ�Τǡ��б���������
// -------------------------------------------------------------------------------
// ���� : filename	: �оݤȤʤ�ե�����̾
// ���� : &result	: �ѥ���ޤ���ե�����̾
int complement_dirname(const char *filename, std::string &result)
{
  int  debug=0;
  if (filename[0]=='/')
    result = filename;
  else if (filename[0]=='.' && filename[1]=='/')
    {
      result = getenv("PWD");
      if (tl_strmember (result.c_str(), ":/cygwin"))
	result = getenv("PWD")+strlen("c:/cygwin");
      result += "/";
      result += filename+2;      // add string with removing "./"
    }
  else if (!strncmp (filename, "../", 3))
    {
      if(debug) tl_message ("input  : %s", filename);
      result = getenv("PWD");
      if (tl_strmember (result.c_str(), ":/cygwin"))
	result = getenv("PWD")+strlen("c:/cygwin");
      result += "/";
      result += filename;
      if(debug) tl_message ("output : %s", result.c_str());
    }
  else if (filename[0]=='.' && filename[1]=='\0')
    {
      result = getenv("PWD");
      if (tl_strmember (result.c_str(), ":/cygwin"))
	result = getenv("PWD")+strlen("c:/cygwin");
      result += "/";
    }
  else
    {
      if(debug) tl_message ("input  : %s", filename);
      result += getenv("MIMESIS_TOP_DIR");
      result += "/";
      result += filename;
      if(debug) tl_message ("output : %s", result.c_str());
    }
  return TRUE;
}



/*-----------------------------------------------------------------------------------*/
// ư�� : �ǥХå��ѥץ���
/*-----------------------------------------------------------------------------------*/
int d_print (char const *format, ...)
{
  char		s[MAX_STRING];
  va_list	list;

  va_start(list, format);
  vsprintf(s, format, list);
  va_end(list);
  fprintf (stderr, "%s", s);
  fflush (stderr);
  return TL_TRUE;
}



/*-----------------------------------------------------------------------------------*/
// ư�� : �ǥХå��ѥץ��� ���Σ�
//        �������� flag �� ON �λ�����ɽ������ */
/*-----------------------------------------------------------------------------------*/
int d_printf (int flag, const char *format, ...)
{
  char		s[MAX_STRING];
  va_list		list;

  if (flag) {
    va_start(list, format);
    vsprintf(s, format, list);
    va_end(list);
    fprintf( stderr, "%s", s );
  }
  return TL_TRUE;
}



/*-----------------------------------------------------------------------------------*/
// ���� : 1999 May 11th
// �ѹ� : 1999 Jul 27th : �ꥹ��ɽ���ϤҤȤޤȤޤ�Ȥ����ɤ߹���褦��
// ư�� : *src ��ʸ�������ˡ�:hoge �Ȥ������� *keyword �����ä����ˤ�
//	  *result �ˤ��μ��� word �򥳥ԡ����ơ�1 ���֤�
//	  ¸�ߤ��ʤ���С�result �� NULL �� 0 ���֤�
// �ѹ� : 1999 Jul 19th : result �� ()�ǰϤޤ�Ƥ����顤�������٤��ɤ�
// ----------------------------------------------------------------------------------
// ���� : *src		: ���������оݤ�ʸ����
// ���� : *keyword	: �������륭��ʸ����
// ���� : *result	: ��̤��Ǽ��������
/*-----------------------------------------------------------------------------------*/
int tl_string_getkeyword (char *src, char *keyword, char *result)
{
  char		*charp=NULL, tmp[MAX_STRING];
  int		i, j, debug=0;

  charp = src;

  if (!tl_strmember(src, keyword))
    {
      tl_warning ("No such keyword {%s} in {%s}", keyword, src );
      result = NULL;
      return TL_FALSE;
    }
  for(;;) {
    sscanf_word_with_pointer_move( &charp, tmp );
    if( !strcmp( tmp, keyword ) ) {
      // ���פ�����
      tl_skip_pointer( &charp );
      if( *charp == '(' ) {
	// Result �κǽ餬 '(' ���ä���� ')' �����ޤ��ɤ�Ǥ��äڤ������
	for( i=0; ;i++ ) {
	  if( *(charp+i)==')'  ) break;
	  if( *(charp+i)=='\0' ) { i--; break; }
	}
	for( j=0; j<=i; j++ ) result[j] = *(charp+j);
	result[j] = '\0';
	//strncpy( result, charp, i+1 );// �������'\0'�ʤ�ʸ������б��Ǥ��ʤ�
	d_printf( debug, "[tetsulib] getkeyword with '()' : i=%d : result=%s\n",
		  i, result );
	return TL_TRUE;
      }
      else {
	// () �ǰϤޤ�Ƥ��ʤ������̤� 1 word �ξ��
	if( sscanf_word_with_pointer_move( &charp, tmp )==TL_TRUE ) {
	  // ���̤� Result ���ä����, �Ǹ���Ĥ����å��������ơ���������
	  strcpy( result, tmp );
	  remove_blacket_end( result );
	  return TL_TRUE;
	}
	else {
	  // ���פ�������Result ��¸�ߤ��Ƥ��ʤ���
	  strcpy(result, "\0");
	  return TL_FAIL;
	}
      }
    }
    else {
      // Keyword ���פ��Ƥ��ʤ���,��ä�õ����
      if( *charp=='\0' ) {
	strcpy(result, "\0");
	return TL_FAIL;
      }
    }
  }
}




/*-----------------------------------------------------------------------------------*/
// ���� : 1999 Jan 21st
// �ѹ� : 1999 Aug 2nd	: reference ������û�������б�������
// ���� : reference		: õ�����оݤ�ʸ����
// ���� : search		: õ������ʸ����
// ư�� : reference ����� search �����뤫�ɤ����������å�����
// ���� : ���ä������֤� reference �β�ʸ���ܤ��ҥåȤ�����
//        (1����Ϥޤ뼫����)   �ʤ����0
/*-----------------------------------------------------------------------------------*/
int tl_strmember (const char *reference, const char *search)
{
  int		i;

  if (reference==NULL || search==NULL)
    return 0;
  // õ�����ʸ���� reference ������û������¨�� return
  if( strlen(reference) < strlen(search) )
    return 0;
  for (i=0; i<(int)strlen(reference)-(int)strlen(search)+1; i++)
    {
      if( !strncmp( reference+i, search, strlen(search) ) )
	return i+1;
    }
  return 0;
}




/*-----------------------------------------------------------------------------------*/
// �ѹ� : 2001 Nov 22nd : ����� ',' ���ڤ�ˤ��б��Ǥ���褦�ˤ���
// ���� : 1999 May 11th 
// ư�� : sscanf ��fscanf ��Ʊ���褦�ˡ��ݥ��󥿤�ư�����ʤ���Ԥʤ��ġ���
//	  int, double �Ǥ˰�³�� word �Ǥ���
// ���� : **charp	: �ɤ߹���٤�ʸ�������Ƭ���ɥ쥹�ؤΥݥ���
// ���� : *word		: �ɤ߼�ä� word
// ���� : (int)		: Success or Fail
/*-----------------------------------------------------------------------------------*/
int sscanf_word_with_pointer_move (char **charp, char *word)
{
  if( **charp=='\0' || **charp=='\n' )
    return TL_FAIL;
  // ʸ����ʸ���Τ����֤򥹥��åפ���褦�ˡ�ʸ����Υݥ��󥿤�ư����
  for(;;) {
    if( **charp==' ' || **charp=='\t' || **charp==',') {
      (*charp)++;
      if( **charp=='\0' || **charp=='\n' )
	return TL_FAIL;
      // ���⤷�ʤ��ǺǸ�ޤǹԤäƤ��ޤä��顤Fail ����
    }
    else break;
  }
  // fgets ���줿ʸ���󤫤� word ���ɤ߼��
  sscanf( *charp, "%s", word );
  // �ɤ߹���� word ��Ĺ��ʬ�������ݥ��󥿤��ư����
  (*charp) += strlen( word );
  return TL_TRUE;
}



/*-----------------------------------------------------------------------------------*/
// �ѹ� : 2001 Nov 22nd : ����� ',' ���ڤ�ˤ��б��Ǥ���褦�ˤ���
// ���� : 1999 Jul 19th
// ư�� : ʸ����ݥ��󥿤��Ф��ơ�����,TAB��skip���Ʋ���ʸ���������ޤǰ�ư����
// ���� : ʸ����ݥ��󥿤Υݥ���
/*-----------------------------------------------------------------------------------*/
int tl_skip_pointer (char **charp)
{
  if( **charp=='\0' ) return TL_FAIL;
  // ʸ����ʸ���Τ����֤򥹥��åפ���褦�ˡ�ʸ����Υݥ��󥿤�ư����
  for(;;) {
    if( **charp==' ' || **charp=='\t' || **charp==',')
      {
	(*charp)++;
	if( **charp=='\0' ) return TL_FAIL;
	// ���⤷�ʤ��ǺǸ�ޤǹԤäƤ��ޤä��顤Fail ����
      }
    else return TL_TRUE;
  }
}



/*-----------------------------------------------------------------------------------*/
// ���� : 1999 Jan 21st
// ư�� : ʸ����κǸ�ˤ��� ')' ���ĺ��
// ���� : �Ǹ�� ')' ������С������򤷤ơ������֤����ʤ���У����֤�
/*-----------------------------------------------------------------------------------*/
int remove_blacket_end( char *string )
{
  if( string[ strlen(string)-1 ] == ')' ) {
    string[ strlen(string)-1 ] = '\0';
    return TL_TRUE;
  }
  return TL_FAIL;
}



/*-----------------------------------------------------------------------------------*/
// ���� : 1999 Sep 15th
// ���� : string	: ���������������ǹ�������Ƥ���Ϥ���ʸ����
// ���� : value		: �Ѵ����줿��
// ���� : Success or Fail
/*-----------------------------------------------------------------------------------*/
int tl_atoi (char *string, int *value)
{
  if (integerp(string)==TL_FALSE)
    {
      my_warning( "tetsulib", "tetsulib_atoi" );
      d_print( "input string is not integer! : %s\n", string );
      return TL_FAIL;
    }
  *value = atoi(string);
  return TL_TRUE;
}



/*-----------------------------------------------------------------------------------*/
// ���� : 1999 Jul 28th
// ư�� : !WARNING! �Ȥ�������ɽ�����ơ����Ƥ��
/*-----------------------------------------------------------------------------------*/
void my_warning (char *process, char *function)
{
  int		i;

  for( i=0; i<5; i++ )
    d_print( "[%s] <%s> !!WARNING!!\n", process, function );
}


/*-----------------------------------------------------------------------------------*/
// ���� : 1999 Jun 16th
// ư�� : ʸ�����ɽ����Ƥ����Τ����������ɤ���Ƚ�Ǥ���
// ���� : *string 	: �о�ʸ����
// ���� : Success or Fail
/*-----------------------------------------------------------------------------------*/
int integerp (char *string)
{
  int		i;

  for (i=0; i<(int)strlen(string); i++)
    {
      if (string[i]<'0' || string[i]>'9')
	if (string[i]!='-')
	  return TL_FALSE;
    }
  return TL_TRUE;
}



/*-----------------------------------------------------------------------------------*/
// �ѹ� : 2001 Nov 22nd : ����� ',' ���ڤ�ˤ��б��Ǥ���褦�ˤ���
// ���� : 1999 Apr 14th
// ư�� : sscanf ��fscanf ��Ʊ���褦�ˡ��ݥ��󥿤�ư�����ʤ���Ԥʤ��ġ���
//        int �Ǥ�³��, double �Ǥ���
// ���� : **charp	: �ɤ߹���٤�ʸ�������Ƭ���ɥ쥹�ؤΥݥ���
// ���� : *data		: �ɤ߼�ä���
// ���� : int		: �������ݤ�
/*-----------------------------------------------------------------------------------*/
int sscanf_double_with_pointer_move (char **charp, double *data)
{
  float		value;
  int		debug=0;

  if(debug) tl_message ("trying ... %s", *charp);
  if( **charp=='\n' || **charp=='\r' || **charp=='\0' )
    {
      if(debug) tl_warning ("End of line. You had better using 'with_seeking'");
      return FALSE;
    }
  // �����ȿ����Τ����֤򥹥��åפ���褦�ˡ�ʸ����Υݥ��󥿤�ư����
  for(;;) {
    if (**charp==' ' || **charp=='\t' || **charp==',') (*charp)++;
    else break;
  }
  if( **charp=='\n' || **charp=='\r' || **charp=='\0' )
    {
      if(debug) tl_warning ("End of line. You had better using 'with_seeking'");
      return FALSE;
    }
  // fgets ���줿ʸ���󤫤龮�������ͤ��ɤ߼��
  sscanf( *charp, "%f", &value );
  // ��������桤�������ʳ��ˤʤ�ޤǥݥ��󥿤��ư����
  for(;;)
    {
      if( (**charp>='0' && **charp<='9') || **charp=='.' || **charp=='-')
	(*charp)++;
      else break;
    }
  *data = (double)value; 
  return TRUE;
}


int Common_SetDebug (int flag)
{
  Gdebug = flag;
  tl_message ("Set Gdebug to %d", flag);
  return flag;
}



//
// Created on 2008-09-27 by inamura
//
int mimesis_init_typetaglist (void)
{
  int debug=1;
  //TypeTagList = g_list_alloc();
  for (int i=0; i<NUM_OF_TYPETAG; i++)
    {
      TypeTagList = g_list_append (TypeTagList, TypeTagString[i]);
      if(debug) tl_message ("%d-th data : %s -> %s", i, TypeTagString[i], (char *)g_list_nth_data (TypeTagList, i));
    }
  return 1;
}


