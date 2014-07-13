//
//  PokerEvaluation.h
//  TestCpp
//
//  Created by 罗耀生 on 14-6-21.
//
//

#ifndef __TestCpp__PokerEvaluation__
#define __TestCpp__PokerEvaluation__


#define BYTE unsigned char
#define GAME_PLAYER 10
#define MAX_COUNT 2
#define MAX_CENTERCOUNT 5
#define FULL_COUNT  52

//扑克类型
enum{
    CT_EMPTY = 0,                                       //空类型
    CT_SINGLE,                                          //单牌类型
    CT_ONE_LONG,                                        //对子类型
    CT_TWO_LONG,                                        //两对类型
    CT_THREE_TIAO,                                      //三条类型
    CT_SHUN_ZI,                                         //顺子类型
    CT_TONG_HUA,                                        //同花类型
    CT_HU_LU,                                           //葫芦类型
    CT_TIE_ZHI,                                         //铁支类型
    CT_TONG_HUA_SHUN,									//同花顺型
    CT_KING_TONG_HUA_SHUN,                              //皇家同花顺
};

class PokerEvaluation {
public:
    /**
     * @brief   排列扑克 -- 从小到大
     */
    static void sortCardList_Evaluation(unsigned char cbCardData[], unsigned char cbCardCount);
    /**
     * @brief   获取数值
     */
    static unsigned char cardValue_Evaluation(unsigned char cbCardData);
    /**
     * @brief   获取花色
     */
    static unsigned char cardColor_Evaluation(unsigned char cbCardData);
    /**
     * @brief   获取逻辑值
     */
    static unsigned char cardLogicValue_Evaluation(unsigned char cbCardData);
    
    /**
     * @brief   获取牌型
     */
    static unsigned char cardType_Evaluation(unsigned char cbCardData[]);
    /**
     * @brief   比牌
     * @pram
     * @return  unsigned char: >0:大于 <0:小于 0:等于
     */
    static int compare_Evaluation(unsigned char cbFirstData[], unsigned char cbNextData[]);
    
    /**
     * @brief   在7张中求最大的5张牌
     */
    static void fiveFromSeven(unsigned char cbHand[2], unsigned char cbCenter[5], unsigned char cbCardTarget[5] );
    
    /**
     * @brief   德州扑克获取最大牌的用户索引数组
     */
    static void maxIdx_Evaluation(unsigned char handCardData[][2], unsigned char curCenterCardData[], int maxIdx[4]);
    
    /**
     * @brief   打印牌数据
     */
    static void ptf_poker_Evaluation(unsigned char );
    static void ptf_type_Evaluation(unsigned char );
};

/**
 * @brief   快速排序使用
 */
int cmp( const void *a , const void *b );

#endif /* defined(__TestCpp__PokerEvaluation__) */
