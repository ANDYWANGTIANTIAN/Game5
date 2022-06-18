#include "storyscene.h"

storyscene::storyscene()
{
}

storyscene::~storyscene()
{
}

storyscene* storyscene::create()
{
	storyscene* pRet = new(std::nothrow)storyscene();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool storyscene::init()
{
	//ÉùÒôÔ¤¼ÓÔØ
	SimpleAudioEngine::getInstance()->preloadEffect("music/fail.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("music/win.wav");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/background.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("music/explode.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("music/shoot.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("music/hitWall.wav");
	auto main_UI = GUIReader::getInstance()->widgetFromJsonFile("001_1/storyscene_1.ExportJson");
	addChild(main_UI);

	auto btn_main_UI = (Button*)main_UI->getChildByTag(2)->getChildByTag(5);
	btn_main_UI->addClickEventListener(CC_CALLBACK_0(storyscene::start, this));

	return true;
}

void storyscene::start()
{
	auto scene = Scene::create();
	auto scene1 = Interface::create();
	scene->addChild(scene1, 2); 
	CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(0.5f, scene,true));
}