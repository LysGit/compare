#include "AppDelegate.h"
#include "GameController.h"
#include "GameData.h"

#include "./C2DXShareSDK/C2DXShareSDK.h"

USING_NS_CC;

using namespace cn::sharesdk;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("texasHoldEmTool");
        director->setOpenGLView(glview);
    }
    
    // turn on display FPS
    director->setDisplayStats( false );
    
    //------------------  auto size -------
    Size screenSize = glview->getFrameSize();
    Size designSize = Size(1136, 640);
    
    FileUtils* pFileUtils = CCFileUtils::getInstance();
    std::vector<std::string> searchPaths;
    
    if ( Platform::OS_IPAD == this->getTargetPlatform() ) {
        designSize = Size(1024, 768);
        searchPaths.push_back("iPad");
        
        memcpy( g_sysPlatform, "iPad", sizeof(char)*strlen("iPad") );
        
    }else if ( Platform::OS_IPHONE == this->getTargetPlatform() ) {
        float visibleSizeWidth = 960.f;
        float visibleSizeHeight =  640.f;

        if ( visibleSizeWidth/visibleSizeHeight ==  screenSize.width/screenSize.height ) {
            designSize = Size(960, 640);
            searchPaths.push_back("iPhone4");
            
            memcpy( g_sysPlatform, "iPhone4", sizeof(char)*strlen("iPhone4") );
            
        }else{
            designSize = Size(1136 , 640);
            searchPaths.push_back("iPhone5");
            
            memcpy( g_sysPlatform, "iPhone5", sizeof(char)*strlen("iPhone5") );
        }
    }else{
        designSize = Size(800 , 480);
        searchPaths.push_back("Android");
        
        memcpy( g_sysPlatform, "Android", sizeof(char)*strlen("Android") );
    }
    
    
    CCLOG("sysPlatform:%s", g_sysPlatform);
    
    searchPaths.push_back("TexasHold'emPoker");
    searchPaths.push_back("LoadingAnimation");
    
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile( "TexasHold'emPoker0.plist" );
    
    pFileUtils->setSearchPaths(searchPaths);
    
    director->setContentScaleFactor(screenSize.height/screenSize.height);
    glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::EXACT_FIT );
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    

    
    std::thread t1(&AppDelegate::loadResources,this);
    t1.join();
    //  t1.detach();
    
    CCLOG("主线程");

    // create a scene. it's an autorelease object
    auto scene = GameController::createScene();
    
    // run
    director->runWithScene(scene);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AppDelegate::loadResources(){
    CCLOG("加载资源文件...");
    
    //添加图片缓存
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile( "TexasHold'emPoker0.plist" );
    
//    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( "Explo.mp3" );
//    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( "rush.mp3" );
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    C2DXShareSDK::open(CCString::create("1cb8ca1c1fec"), false);//ios
//#else
//    C2DXShareSDK::open(CCString::create("1caa06ec06ec"), false);//Android
//#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    C2DXShareSDK::open(CCString::create("1cb8ca1c1fec"), false);//ios
#else
    C2DXShareSDK::open(CCString::create("1caa06ec06ec"), false);//Android
#endif

    //新浪微博
    __Dictionary *sinaConfigDict = CCDictionary::create();
    sinaConfigDict -> setObject(CCString::create("2986166641"), "app_key");
    sinaConfigDict -> setObject(CCString::create("879bb0323253e6457da60f2d106ded01"), "app_secret");
    sinaConfigDict -> setObject(CCString::create("http://hi.baidu.com/luoyaosheng"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSinaWeibo, sinaConfigDict);
    
    //微信
    __Dictionary *wcConfigDict = CCDictionary::create();
    wcConfigDict -> setObject(CCString::create("wx724b328830ae871c"), "app_id");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiSession, wcConfigDict);
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiTimeline, wcConfigDict);
    //    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiFav, wcConfigDict);
    
    //QQ
    __Dictionary *qqConfigDict = CCDictionary::create();
    qqConfigDict -> setObject(CCString::create("101074973"), "app_id");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeQQ, qqConfigDict);
    
    //QQ空间
    __Dictionary *qzConfigDict = CCDictionary::create();
    qzConfigDict -> setObject(CCString::create("100371282"), "app_id");
    qzConfigDict -> setObject(CCString::create("ef77a107b960b53583d14481b8a3ddf9"), "app_key");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeQZone, qzConfigDict);
    
    //腾讯微博
    __Dictionary *tcConfigDict = CCDictionary::create();
    tcConfigDict -> setObject(CCString::create("801501543"), "app_key");
    tcConfigDict -> setObject(CCString::create("b7088106071bac5c77c4837e747749d2"), "app_secret");
    tcConfigDict -> setObject(CCString::create("http://www.mbp.com"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeTencentWeibo, tcConfigDict);
}

