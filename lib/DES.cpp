#include <QQMusic/DES.hpp>
#define DESENCRY 0
#define DESDECRY 1
namespace QMT{
  void DES::des(char *data, char *key,int readlen)
  {
  	int i = 0;
  	makefirstkey((unsigned int*)key);
  	for (i = 0; i < readlen; i += 8)
  	{
  		handle_data((unsigned int*)&data[i], DESENCRY);
  	}
  }

  void DES::Ddes(char *data,char *key,int readlen)
  {
  	int i = 0;
  	makefirstkey((unsigned int*)key);
  	for (i = 0; i < readlen; i += 8)
  	{
  		handle_data((unsigned int*)&data[i] ,DESDECRY);
  	}
  }

  void DES::handle_data(unsigned int *left , int choice)
  {
  	int  number = 0 ,j = 0;
  	unsigned int *right = &left[1];
  	unsigned int tmp = 0;
  	unsigned int tmpbuf[2] = { 0 };

  	for ( int i = 0; i < 64; ++i )
     {
       if ( i >= 32 )
       {
         if ( wz_pc1[i] <= 32 )
         {
           if ( wz_pc2[wz_pc1[i] - 1] & *left )
             tmpbuf[1] |= wz_pc2[i];
         }
         else if ( wz_pc2[wz_pc1[i] - 1] & *right )
         {
           tmpbuf[1] |= wz_pc2[i];
         }
       }
       else if ( wz_pc1[i] <= 32 )
       {
         if ( wz_pc2[wz_pc1[i] - 1] & *left )
           tmpbuf[0] |= wz_pc2[i];
       }
       else if ( wz_pc2[wz_pc1[i] - 1] & *right )
       {
         tmpbuf[0] |= wz_pc2[i];
       }
     }
  	*left  = tmpbuf[0];
  	*right = tmpbuf[1];
  	tmpbuf[0] = 0;
  	tmpbuf[1] = 0;
      switch(choice )
  	{
  		case DESENCRY:
  			for (number = 0; number < 16; number++)
  			{
  				makedata(left , right , (unsigned long)number);
  			}
  			break;
  		case DESDECRY:
  			for (number = 15; number >= 0; number--)
  			{
  				makedata(left , right ,(unsigned long)number);
  			}
  			break;
  		default:
  			break;
  	}

  	tmp = *left;
  	*left = *right;
  	*right = tmp;
  	for ( int l = 0; l < 64; ++l )
    {
      if ( l >= 32 )
      {
        if ( wz_pc4[l] <= 32 )
        {
          if ( wz_pc2[wz_pc4[l] - 1] & *left )
            tmpbuf[1] |= wz_pc2[l];
        }
        else if ( wz_pc2[wz_pc4[l] - 1] & *right )
        {
          tmpbuf[1] |= wz_pc2[l];
        }
      }
      else if ( wz_pc4[l] <= 32 )
      {
        if ( wz_pc2[wz_pc4[l] - 1] & *left )
          tmpbuf[0] |= wz_pc2[l];
      }
      else if ( wz_pc2[wz_pc4[l] - 1] & *right )
      {
        tmpbuf[0] |= wz_pc2[l];
      }
    }

  	*left =  tmpbuf[0];
  	*right = tmpbuf[1];
  }


  void DES::makedata(unsigned int  *left ,unsigned int  *right ,unsigned int number)
  {
  	unsigned int oldright = *right;
  	char rexpbuf[8] = { 0};
  	unsigned int datatmp = 0;
  	unsigned int exp[2] = { 0};

  	for (int i = 0; i < 48; i++)
  	{
  		if ( i >= 24 )
      {
        if ( wz_pc2[exptab3[i] - 1] & *right )
          exp[1] |= wz_pc2[i - 24];
      }
      else if ( wz_pc2[exptab3[i] - 1] & *right )
      {
        exp[0] |= wz_pc2[i];
      }
  	}

  	for (int j = 0; j < 2; j++)
  	{
  		exp[j] ^= g_outkey[number][j];
  	}

  	exp[1] >>= 8;
  	  rexpbuf[7] = exp[1] & 0x3F;
  	  exp[1] >>= 6;
  	  rexpbuf[6] = exp[1] & 0x3F;
  	  exp[1] >>= 6;
  	  rexpbuf[5] = exp[1] & 0x3F;
  	  exp[1] >>= 6;
  	  rexpbuf[4] = exp[1] & 0x3F;
  	  exp[0] >>= 8;
  	  rexpbuf[3] = exp[0] & 0x3F;
  	  exp[0] >>= 6;
  	  rexpbuf[2] = exp[0] & 0x3F;
  	  exp[0] >>= 6;
  	  rexpbuf[1] = exp[0] & 0x3F;
  	  exp[0] >>= 6;
  	  rexpbuf[0] = exp[0] & 0x3F;
  	  exp[0] = 0;
  	  exp[1] = 0;

  	*right = 0;
  	int k=0;
  	for (k = 0; k < 7; k++)
  	{
  		*right |= SP[k][rexpbuf[k]];
  		*right <<= 4;
  	}
  	*right |= SP[k][rexpbuf[k]];

  	datatmp = 0;
  	for (int j = 0; j < 32; j++)
  	{
  		if (*right&wz_pc2[wz_pc3[j]-1] )
  		{
  			datatmp |= wz_pc2[j];
  		}
  	}
  	*right = datatmp;

  	*right ^= *left;
  	*left = oldright;
  }

  void DES::makefirstkey(unsigned int *keyP )
  {
  	unsigned int key[2] = {0};
  	int j;
  	InitIntArray(g_bufkey, 2);
  	copyFromIntArray(key, keyP, 2);

  	InitIntArray2(g_outkey, 16, 2);

  	for(j = 0; j < 28; j++)
  	{
  		if ( wz_keyleft[j] <= 32 )
  	 {
  		 if ( wz_pc2[wz_keyleft[j] - 1] & key[0] )
  			 g_bufkey[0] |= wz_pc2[j];
  	 }
  	 else if ( wz_pc2[wz_keyleft[j] - 1] & key[1] )
  	 {
  		 g_bufkey[0] |= wz_pc2[j];
  	 }
  	 if ( wz_keyright[j] <= 32 )
  	 {
  		 if ( wz_pc2[wz_keyright[j] - 1] & key[0] )
  			 g_bufkey[1] |= wz_pc2[j];
  	 }
  	 else if ( wz_pc2[wz_keyright[j] - 1] & key[1] )
  	 {
  		 g_bufkey[1] |= wz_pc2[j];
  	 }
  	}
  	for (j = 0; j < 16; j++)
  	{
  		makekey(&g_bufkey[0],&g_bufkey[1],j);
  	}
  }

  void DES::InitIntArray(unsigned int buf[], int nSize)
  {
  	for ( int i = 0; i < nSize; i++)
  	{
  		buf[i] = 0;
  	}
  }

  void DES::InitIntArray2(unsigned int buf[][2], int nSize1, int nSize2)
  {
  	for ( int i = 0; i < nSize1; i++)
  	{
  		for (int j = 0; j < nSize2; j++)
  		{
  			buf[i][j] = 0;
  		}
  	}
  }

  void DES::copyFromIntArray(unsigned int toBuf[], unsigned int fromBuf[], int nSize)
  {
  	for ( int i = 0; i < nSize; i++)
  	{
  		toBuf[i] = fromBuf[i];
  	}
  }

  void DES::makekey(unsigned int *keyleft,unsigned int *keyright ,unsigned int number)
  {
  	unsigned int tmpkey[2] ={0};
  	unsigned int *Ptmpkey = (unsigned int*)tmpkey;
  	unsigned int *Poutkey = (unsigned int*)&g_outkey[number];
  	int j;

  	InitIntArray(tmpkey, 2);

  	*Ptmpkey = *keyleft&wz_leftandtab[wz_lefttable[number]];
  	Ptmpkey[1] = *keyright&wz_leftandtab[wz_lefttable[number]];
  	if (wz_lefttable[number] == 1)
  	{
  		*Ptmpkey >>= 27;
  		Ptmpkey[1] >>= 27;
  	}
  	else
  	{
  		*Ptmpkey >>= 26;
  		Ptmpkey[1] >>= 26;
  	}
  	Ptmpkey[0] &= 0xfffffff0;
  	Ptmpkey[1] &= 0xfffffff0;

  	*keyleft <<= wz_lefttable[number];
  	*keyright <<= wz_lefttable[number];
  	*keyleft |= Ptmpkey[0];
  	*keyright |= Ptmpkey[1];
  	Ptmpkey[0] = 0;
  	//Ptmpkey[1] = 0;

  	for (j = 0; j < 48; j++)
  	{
  		if ( j >= 24 )
  	     {
  	       if ( wz_pc2[wz_keychoose[j] - 28] & *keyright ){
  					Poutkey[1] |= wz_pc2[j - 24];
  				 }

  	     }
  	     else if ( wz_pc2[wz_keychoose[j] - 1] & *keyleft )
  	     {
  	       *Poutkey |= wz_pc2[j];
  	     }
  	}

  }
}
