//
//  GameController.cpp
//  texasHoldEmTool
//
//  Created by 罗耀生 on 14-7-5.
//
//

#include "GameController.h"
#include "GameData.h"
using namespace cocos2d;

Scene* GameController::createScene()
{
    auto scene = Scene::create();
    auto layer = GameController::create();
    scene->addChild(layer);
    return scene;
}

bool GameController::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->DATA_init();
    
    this->loadJsonFile();
    
    std::thread t1(&GameController::startLoad,this);
    t1.join();
    
    this->endLoad();

    
    return true;
}

void GameController::loadJsonFile(){
    
    std::string str = "TexasHold'emPoker_";
    str += sysPlatform;
    str += ".ExportJson";
    Layout* widget = (Layout*)GUIReader::getInstance()->widgetFromJsonFile( str.c_str() );
    this->addChild( widget );
    widget->setBackGroundImage( "bg.jpg" );
    
    _panel_info  = static_cast<Layout*>(widget->getChildByName( "Panel_info" ));
    _panel_play  = static_cast<Layout*>(widget->getChildByName( "Panel_play" ));
    _panel_choose  = static_cast<Layout*>(widget->getChildByName( "Panel_choose" ));
    _panel_help  = static_cast<Layout*>(widget->getChildByName( "Panel_help" ));
    _panel_explain  = static_cast<Layout*>(widget->getChildByName( "Panel_explain" ));
//    _panel_menu  = static_cast<Layout*>(widget->getChildByName( "Panel_menu" ));
    _panel_teach  = static_cast<Layout*>(widget->getChildByName( "Panel_teach" ));
    
    
    Button *button_help = static_cast<Button*>(widget->getChildByName( "Button_help" ));
    Button *button_explain = static_cast<Button*>(widget->getChildByName( "Button_explain" ));
    Button *button_share = static_cast<Button*>(widget->getChildByName( "Button_share" ));
    Button *button_refresh = static_cast<Button*>(widget->getChildByName( "Button_refresh" ));
    
    button_help->addTouchEventListener( CC_CALLBACK_2(GameController::selectEvent_menu, this) );
    button_explain->addTouchEventListener( CC_CALLBACK_2(GameController::selectEvent_menu, this) );
    button_share->addTouchEventListener( CC_CALLBACK_2(GameController::selectEvent_menu, this) );
    button_refresh->addTouchEventListener( CC_CALLBACK_2(GameController::selectEvent_menu, this) );
    
    for ( auto& e : _panel_play->getChildren() ) {
        Button *btn = (Button *)e;
        btn->addTouchEventListener( CC_CALLBACK_2(GameController::selectEvent_play, this) );
    }
    
    for ( auto& e : _panel_choose->getChildren() ) {
        Button *btn = (Button *)e;
        btn->addTouchEventListener( CC_CALLBACK_2(GameController::selectEvent_choose, this) );
    }
    
    for (int idx = 0; idx < 5; idx++) {
        std::string s = StringUtils::format("poker_%d", idx+100) ;
        _choose_button[ idx ] = static_cast<Button*>(_panel_choose->getChildByName( s.c_str() ));
        
        _choose_button[ idx ]->setTag( 999 );
    }
}

void GameController::startLoad(){
    CCLOG("加载等待动画");
}
void GameController::endLoad(){
    CCLOG("关闭等待动画");
}

#pragma mark - Data init
void GameController::DATA_init(){
    _iTouchIdx = 0;
}

#pragma mark - Action set
void GameController::selectEvent_menu(Ref *pSender, Widget::TouchEventType type){
    
    if ( TOUCH_EVENT_BEGAN == (int)type) {
        
        Button *btn = (Button *)pSender;
        
        switch ( btn->getTag() ) {
            case 10:
            {
                CCLOG("帮助");
            }
                break;
            case 11:
            {
                CCLOG("说明");
                
            }
                break;
            case 12:
            {
                CCLOG("分享");
            }
                break;
            default:
            {
                CCLOG("刷新");
            }
                break;
        }
        
    }
}

void GameController::selectEvent_choose(Ref *pSender, Widget::TouchEventType type){
    
    if ( TOUCH_EVENT_BEGAN == (int)type) {
        
        Button *btn = (Button *)pSender;
        std::string s = btn->getName();
        int tag = atoi( s.substr( s.find('_')+1, s.size() ).c_str() );
        switch ( tag ) {
            case 0:
            {
                _panel_choose->setVisible( false );
            }
                break;
                
            default:
            {
                if ( tag >= 100) {
                    _iTouchIdx = tag - 100;
                    
                    if ( _choose_button[ _iTouchIdx ]->getTag() != 999) {

                        std::string s2 = StringUtils::format("poker_%d", _choose_button[ _iTouchIdx ]->getTag()) ;
                        Button *btn1 = (Button *)_panel_choose->getChildByName( s2.c_str() );
                        btn1->setBright( true );
                        
                        _choose_button[ _iTouchIdx ]->setVisible( true );
                        _choose_button[ _iTouchIdx ]->loadTextures( "poker70.png", "","",  TextureResType::PLIST);
                        _choose_button[ _iTouchIdx ]->setTag( 999 );
                    }
                }else{
                    if ( _iTouchIdx > 4) {
                        return;
                    }
                    
                    btn->setBright( false );
                    
                    std::string s1 = StringUtils::format("poker%d.png", tag) ;
                    _choose_button[ _iTouchIdx ]->loadTextures( s1.c_str(), "poker70.png", "", TextureResType::PLIST);
                    _choose_button[ _iTouchIdx ]->setTag( tag );
                    _iTouchIdx++;
                }
            }
                break;
        }
    }
    //截取字符
}

void GameController::selectEvent_play(Ref *pSender, Widget::TouchEventType type){
    
    if ( TOUCH_EVENT_BEGAN == (int)type) {
        
        _iTouchIdx = 0;
        _panel_choose->setVisible( true );

        Button *btn = (Button *)pSender;
        std::string s = btn->getName();
        int tag = atoi( s.substr( s.find('_')+1, s.size() ).c_str() );
        int iNum = 10==tag?5:2;
        
        //动画
        Point pt[5];
        for (int idx = 0; idx < 5; idx++) {
            pt[ idx ] = _choose_button[ idx ]->getPosition();

            if ( idx < iNum ) {
                _choose_button[ idx ]->setVisible( true );
            }else{
                _choose_button[ idx ]->setVisible( false );
            }
            
            _choose_button[ idx ]->loadTextures( "poker70.png", "","",  TextureResType::PLIST);
        }
        
//        for (int idx = 0; idx < 5; idx++) {
//            
//            button[ idx ]->setPosition( pt[2] );
//            
//            MoveTo *to = MoveTo::create(2, pt[ idx ] );
//            button[ idx ]->runAction( to );
//        }
        
    }
}
