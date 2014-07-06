//
//  GameController.h
//  texasHoldEmTool
//
//  Created by 罗耀生 on 14-7-5.
//
//

#ifndef __texasHoldEmTool__GameController__
#define __texasHoldEmTool__GameController__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocostudio;
using namespace cocos2d::ui;

class GameController : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC( GameController );
    
    virtual void loadJsonFile();
    virtual void startLoad();
    virtual void endLoad();
    
    virtual void DATA_init();
    //ACT
    void selectEvent_menu(Ref *pSender, Widget::TouchEventType type);
    void selectEvent_choose(Ref *pSender, Widget::TouchEventType type);
    void selectEvent_play(Ref *pSender, Widget::TouchEventType type);

private:
    Layout *_panel_info;
    Layout *_panel_play;
    Layout *_panel_choose;
    Layout *_panel_help;
    Layout *_panel_explain;
//    Layout *_panel_menu;
    Layout *_panel_teach;
    
    Button *_choose_button[5];
    
    
    int _iTouchIdx;
};

#endif /* defined(__texasHoldEmTool__GameController__) */
