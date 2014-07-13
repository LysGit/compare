//
//  PokerEvaluation.cpp
//  TestCpp
//
//  Created by 罗耀生 on 14-6-21.
//
//

#include "PokerEvaluation.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

//0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
//0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
//0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
//0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K


//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

inline int cmp( const void *a , const void *b )
{
    return *(unsigned char *)a - *(unsigned char *)b;
}

void PokerEvaluation::sortCardList_Evaluation(unsigned char cbCardData[], unsigned char cbCardCount)
{
    qsort( cbCardData, cbCardCount, sizeof( cbCardData[0] ), cmp);
}

unsigned char PokerEvaluation::cardValue_Evaluation(unsigned char cbCardData)
{
    return cbCardData&LOGIC_MASK_VALUE;
}

unsigned char PokerEvaluation::cardColor_Evaluation(unsigned char cbCardData)
{
    return cbCardData&LOGIC_MASK_COLOR;
}

unsigned char PokerEvaluation::cardLogicValue_Evaluation(unsigned char cbCardData)
{
	unsigned char bCardValue = cardValue_Evaluation(cbCardData);
	return (bCardValue==1)?(bCardValue+13):bCardValue;
}

unsigned char PokerEvaluation::cardType_Evaluation(unsigned char cbCardData[])
{
    unsigned char ucColor[5] = {0};
    unsigned char ucValue[5] = {0};
    unsigned char ucLogicValue[5] = {0};
    
    for (int idx = 0; idx < 5; idx++) {
        
        ucColor[ idx ] = PokerEvaluation::cardColor_Evaluation( cbCardData[ idx ] );
        ucValue[ idx ] = PokerEvaluation::cardValue_Evaluation( cbCardData[ idx ] );
        ucLogicValue[ idx ] = PokerEvaluation::cardLogicValue_Evaluation( cbCardData[ idx ] );
    }

    PokerEvaluation::sortCardList_Evaluation(ucColor, 5);
    PokerEvaluation::sortCardList_Evaluation(ucValue, 5);
    PokerEvaluation::sortCardList_Evaluation(ucLogicValue, 5);
    
    bool bTongHua = false;
    bool bSHunZi = false;

    if ( ucColor[0] == ucColor[1] && ucColor[1] == ucColor[2] && ucColor[2] == ucColor[3] && ucColor[3] == ucColor[4] ) {
        bTongHua = true;
    }
    
    if (  ( (ucValue[1]-ucValue[0] == ucValue[2]-ucValue[1])
           &&  (ucValue[2]-ucValue[1] == ucValue[3]-ucValue[2])
           &&  (ucValue[4]-ucValue[3] == ucValue[3]-ucValue[2]) )
        || ( (ucLogicValue[1]-ucLogicValue[0] == ucLogicValue[2]-ucLogicValue[1])
            &&  (ucLogicValue[2]-ucLogicValue[1] == ucLogicValue[3]-ucLogicValue[2])
            &&  (ucLogicValue[4]-ucLogicValue[3] == ucLogicValue[3]-ucLogicValue[2]) ) ) {
        bSHunZi = true;
    }
    
    if ( !bSHunZi ) {
        if ( ucValue[0] == ucValue[1] || ucValue[1] == ucValue[2] || ucValue[2] == ucValue[3] || ucValue[3] == ucValue[4]) {
            if ( ucValue[0] == ucValue[1] == ucValue[2] == ucValue[3] == ucValue[4] ) {
                return CT_TIE_ZHI;
            }else if ( (ucValue[0] == ucValue[1] == ucValue[2] ) || (ucValue[1] == ucValue[2] == ucValue[3] ) || (ucValue[2] == ucValue[3] == ucValue[4] ) ){
                
                if ( ( (ucValue[0] == ucValue[1] == ucValue[2] ) && (ucValue[3] == ucValue[4] ) )
                    || ( (ucValue[1] == ucValue[2] == ucValue[3] ) && (ucValue[0] == ucValue[4] ) )
                    || ( (ucValue[2] == ucValue[3] == ucValue[4] ) && (ucValue[1] == ucValue[2] ) )) {
                    return CT_HU_LU;
                }else{
                    return CT_THREE_TIAO;
                }
            }else if ( ( ucValue[0] == ucValue[1] && ucValue[2] == ucValue[4] )
                      || ( ucValue[0] == ucValue[1] && ucValue[3] == ucValue[4] )
                      || ( ucValue[1] == ucValue[2] && ucValue[3] == ucValue[4] ) )
            {
                return CT_TWO_LONG;
            }else{
                return CT_ONE_LONG;
            }
        }
    }
    

    if ( bSHunZi ) {
        if ( bTongHua ) {
            if ( ucLogicValue[0] == 14 ) {
                return CT_KING_TONG_HUA_SHUN;
            }else{
                return CT_TONG_HUA_SHUN;
            }
        }else{
            return CT_SHUN_ZI;
        }
    }
    
    if ( bTongHua ) {
        return CT_TONG_HUA;
    }
    
    return CT_SINGLE;
}

//1:大于 -1:小于 0:等于
int PokerEvaluation::compare_Evaluation(unsigned char cbFirstData[], unsigned char cbNextData[])
{
    //判断类型
    int type = PokerEvaluation::cardType_Evaluation(cbFirstData) - PokerEvaluation::cardType_Evaluation(cbNextData);
    
    if ( type > 0) {
        return 1;
    }else if ( type < 0) {
        return -1;
    }else{

        unsigned char ucLogicValue[2][5] = {0};
        
        for (int j = 0; j < 5; j++) {
            ucLogicValue[0][j] = PokerEvaluation::cardLogicValue_Evaluation( cbFirstData[ j ] );
            ucLogicValue[1][j] = PokerEvaluation::cardLogicValue_Evaluation( cbNextData[ j ] );
        }
        
        for (int i = 0; i < 2; i++) {
            PokerEvaluation::sortCardList_Evaluation( ucLogicValue[i], 5);
        }
        
        switch ( type ) {
            case CT_EMPTY:
            case CT_SINGLE:
            case CT_SHUN_ZI:
            case CT_TONG_HUA:
            case CT_TONG_HUA_SHUN:
            case CT_KING_TONG_HUA_SHUN:
            {
                for (int i = 0; i < 5; i++) {
                    int t = ucLogicValue[0][4-i]-ucLogicValue[1][4-i];
                    if ( i == 4) {
                        return t;
                    }
                    if ( t != 0) {
                        return t;
                    }
                }
            }
                break;
            case CT_TIE_ZHI:
            {
                int idx[2][2] = {0};
                if ( ucLogicValue[0][0] != ucLogicValue[0][1] ) {
                    idx[0][0] = 4;
                    idx[0][1] = 0;
                }else{
                    idx[0][0] = 0;
                    idx[0][1] = 4;
                }
                if ( ucLogicValue[1][0] != ucLogicValue[1][1] ) {
                    idx[0][0] = 0;
                    idx[0][1] = 4;
                }else{
                    idx[0][0] = 4;
                    idx[0][1] = 0;
                }
                
                for (int i = 0; i < 2; i++) {
                    int t = ucLogicValue[0][ idx[0][i] ]-ucLogicValue[1][ idx[1][i] ];
                    if ( i == 1) {
                        return t;
                    }
                    if ( t != 0) {
                        return t;
                    }
                }
            }
                break;
            case CT_ONE_LONG:
            {
                int twoIdx[2] = {0};
                int oneIdx[2][3] = {0};
                
                for (int i = 0; i < 5; i++) {
                    for (int j = i+1; j < 5; j++) {
                        if ( ucLogicValue[0][i] == ucLogicValue[0][j] ) {
                            twoIdx[0] = i;
                            int r = 0;
                            for (int w = 0; w < 5; w++) {
                                if ( w != i && w != j) {
                                    oneIdx[0][r++] = 4-w;
                                }
                            }
                        }
                        if ( ucLogicValue[1][i] == ucLogicValue[1][j] ) {
                            twoIdx[1] = i;
                            int r = 0;
                            for (int w = 0; w < 5; w++) {
                                if ( w != i && w != j) {
                                    oneIdx[1][r++] = 4-w;
                                }
                            }
                        }
                    }
                }
                
                //判断大小
                for (int i = 0; i < 3; i++) {
                    int t = ucLogicValue[0][ oneIdx[0][i] ]-ucLogicValue[1][ oneIdx[1][i] ];
                    if ( i == 2) {
                        return t;
                    }
                    if ( t != 0) {
                        return t;
                    }
                }
            }
                break;
            case CT_TWO_LONG:
            {
                int idx[2][3];
                
                if ( ucLogicValue[0][0] != ucLogicValue[0][1] ) {
                    idx[0][2] = 0;
                    idx[0][0] = 3;
                    idx[0][1] = 1;
                }else{
                    if ( ucLogicValue[0][2] != ucLogicValue[0][1] ) {
                        idx[0][2] = 2;
                        idx[0][0] = 3;
                        idx[0][1] = 0;
                    }else{
                        idx[0][2] = 4;
                        idx[0][0] = 2;
                        idx[0][1] = 0;
                    }
                }
                
                if ( ucLogicValue[1][0] != ucLogicValue[1][1] ) {
                    idx[1][2] = 0;
                    idx[1][0] = 3;
                    idx[1][1] = 1;
                }else{
                    if ( ucLogicValue[1][2] != ucLogicValue[1][1] ) {
                        idx[1][2] = 2;
                        idx[1][0] = 3;
                        idx[1][1] = 0;
                    }else{
                        idx[1][2] = 4;
                        idx[1][0] = 2;
                        idx[1][1] = 0;
                    }
                }

                for (int i = 0; i < 3; i++) {
                    int t = ucLogicValue[0][ idx[0][i] ]-ucLogicValue[1][ idx[1][i] ];
                    if ( i == 2) {
                        return t;
                    }
                    if ( t != 0) {
                        return t;
                    }
                }
            }
                break;
            case CT_HU_LU:
            case CT_THREE_TIAO:
            {
                //找出三条和一对位置
                int threeIdx[2] = {0};
                int twoIdx[2] = {0};

                if ( ucLogicValue[0][0]==ucLogicValue[0][2] ) {
                    threeIdx[0] = 0;
                    twoIdx[0] = 3;
                }else{
                    threeIdx[0] = 3;
                    twoIdx[0] = 0;
                }
                
                if ( ucLogicValue[1][0]==ucLogicValue[1][2] ) {
                    threeIdx[1] = 0;
                    twoIdx[1] = 3;
                }else{
                    threeIdx[1] = 3;
                    twoIdx[1] = 0;
                }
                
                //判断大小
                int t = ucLogicValue[0][ threeIdx[0] ] - ucLogicValue[1][ threeIdx[1] ];
                if ( t == 0  ) {
                    //分类处理
                    if ( CT_HU_LU == type) {
                        t = ucLogicValue[0][ twoIdx[0] ] - ucLogicValue[1][ twoIdx[1] ];
                        return t;
                    }else{
                        t = ucLogicValue[0][ twoIdx[0] ] - ucLogicValue[1][ twoIdx[1] ];
                        if ( t == 0 ) {
                            t = ucLogicValue[0][ twoIdx[0]+1 ] - ucLogicValue[1][ twoIdx[1]+1 ];
                            return t;
                        }else{
                            return t;
                        }
                    }
                }else{
                    return t;
                }
            }
                break;

            default:
                printf("说，你怎么跑进来的！！！");
                break;
        }
        
    }
    
    return 0;
}

/**
 * @brief   在7张中求最大的5张牌
 */
void PokerEvaluation::fiveFromSeven(unsigned char cbHand[2], unsigned char cbCenter[5], unsigned char cbCardTarget[5] )
{
    unsigned char cbCardSource[7] = { cbHand[0], cbHand[1] ,cbCenter[0], cbCenter[1] ,cbCenter[2], cbCenter[3] ,cbCenter[4] };
    unsigned char cbTmp[5] = {0};

    int m = 5;
    int n = 7;
    
    for (int i = 0; i < m; i++) {
        cbCardTarget[i] = cbCardSource[i];
    }
    
    //组合算法 -- 自己抒写，暂时未测
    for (int i = 0; i < n; i++) {
        int r = 0;
        for (int j = i; j < m+i; j++) {
            int k = j>=n?j-n:j;
            cbTmp[r++] = cbCardSource[k];
        }

        if ( PokerEvaluation::compare_Evaluation( cbCardTarget, cbTmp) < 0) {
            for (int z = 0; z < m; z++) {
                cbCardTarget[z] = cbTmp[z];
            }
        }
    }
}

void PokerEvaluation::maxIdx_Evaluation(unsigned char handCardData[][2], unsigned char curCenterCardData[], int maxIdx[4])
{
    for (int i = 0; i < 4; i++) {
        maxIdx[i] = -1;
    }
    unsigned char cardTag[GAME_PLAYER][5];
    for (int i = 0; i < GAME_PLAYER; i++) {
        for (int j = 0; j < 5; j++) {
            cardTag[i][j] = 0;
        }
    }

    //求出7返5最大牌
    for (int i = 0; i < GAME_PLAYER; i++) {
        PokerEvaluation::fiveFromSeven( handCardData[i], curCenterCardData, cardTag[i] );
    }
    
    //求出最大牌位置
    int imax = 0;
    for (int i = 0; i < GAME_PLAYER; i++) {
        if ( handCardData[i][0] == 0 || handCardData[i][1] == 0 ) {
            continue;
        }

        if ( PokerEvaluation::compare_Evaluation( cardTag[i], cardTag[imax] ) > 0 ) {
            imax = i;
        }
    }

    //找出相同索引
    int t = 0;
    for (int i = 0; i < GAME_PLAYER; i++) {
        if ( handCardData[i][0] == 0 || handCardData[i][1] == 0 ) {
            continue;
        }
        
        if ( PokerEvaluation::compare_Evaluation( cardTag[i], cardTag[imax] ) == 0 ) {
            maxIdx[t++] = i;
        }
    }
}


void PokerEvaluation::ptf_poker_Evaluation(unsigned char card)
{
    unsigned char color = cardColor_Evaluation( card );
    unsigned char value = cardLogicValue_Evaluation( card );

    switch ( color ) {
        case 0x00:
            printf(" ♦️");
            break;
        case 0x10:
            printf(" ♣️");
            break;
        case 0x20:
            printf(" ♥️");
            break;
        default:
            printf(" ♠️");
            break;
    }
    
    if ( value < 11) {
        printf("%d ", value);
    }else{
        switch ( value ) {
            case 11:
                printf("J ");
                break;
            case 12:
                printf("Q ");
                break;
            case 13:
                printf("K ");
                break;
            default:
                printf("A ");
                break;
        }
    }
}

void PokerEvaluation::ptf_type_Evaluation(unsigned char type)
{
    switch ( type ) {
        case CT_SINGLE:
            printf("单牌类型");
            break;
            
        case CT_ONE_LONG:
            printf("对子类型");
            break;
            
        case CT_TWO_LONG:
            printf("两对类型");
            break;
            
        case CT_THREE_TIAO:
            printf("三条类型");
            break;
            
        case CT_SHUN_ZI:
            printf("顺子类型");
            break;
            
        case CT_TONG_HUA:
            printf("同花类型");
            break;
            
        case CT_HU_LU:
            printf("葫芦类型");
            break;
            
        case CT_TIE_ZHI:
            printf("铁支类型");
            break;
            
        case CT_TONG_HUA_SHUN:
            printf("同花顺型");
            break;
            
        case CT_KING_TONG_HUA_SHUN:
            printf("皇家同花顺");
            break;
            
        default:
            printf("空类型");
            break;
    }
    printf("  ");
}
