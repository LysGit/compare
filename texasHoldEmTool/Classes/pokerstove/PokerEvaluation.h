//
//  PokerEvaluation.h
//  TestCpp
//
//  Created by 罗耀生 on 14-6-21.
//
//

#ifndef __TestCpp__PokerEvaluation__
#define __TestCpp__PokerEvaluation__

class PokerEvaluation {
public:
    /**
     * @brief   排列扑克 -- 从大到小
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
     * @brief   打印牌数据
     */
    static void ptf_poker_Evaluation(unsigned char );
};

/**
 * @brief   快速排序使用
 */
int cmp( const void *a , const void *b );

#endif /* defined(__TestCpp__PokerEvaluation__) */
