#include "AppDelegate.h"
#include "GameController.h"

USING_NS_CC;

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
    director->setDisplayStats(true);
    
    //------------------  shipei -------
    Size screenSize = glview->getFrameSize();
    Size designSize = Size(640, 1136);
    
    FileUtils* pFileUtils = CCFileUtils::getInstance();
    std::vector<std::string> searchPaths;
    
    if ( Platform::OS_IPAD == this->getTargetPlatform() ) {
        designSize = Size(1024, 768);
        searchPaths.push_back("iPad");
    }else if ( Platform::OS_IPHONE == this->getTargetPlatform() ) {
        float visibleSizeWidth = 640.f ;
        float visibleSizeHeight = 960.f ;

        if ( visibleSizeWidth/visibleSizeHeight ==  screenSize.width/screenSize.height ) {
            designSize = Size(640, 960);
            searchPaths.push_back("iPhone4");
        }else{
            designSize = Size(640, 1136);
            searchPaths.push_back("iPhone5");
        }
    }else{
        designSize = Size(480, 800);
        searchPaths.push_back("Android");
    }
    
//    searchPaths.push_back("Particle");
    
    
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
//    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( "Explo.mp3" );
//    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect( "rush.mp3" );
}
