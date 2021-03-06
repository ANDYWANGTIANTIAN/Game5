#include "Interface.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

using namespace std;

int Interface::tool;

string GBKToUTF8(const std::string& strGBK)
{
	string strOutUTF8 = "";
	WCHAR* str1;
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
	str1 = new WCHAR[n];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char* str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
	strOutUTF8 = str2;
	delete[]str1;
	str1 = NULL;
	delete[]str2;
	str2 = NULL;
	return strOutUTF8;
}

BattleScene* Interface::gamescene = nullptr;

Interface::Interface()
{
}

Interface::~Interface()
{
}

Interface* Interface::create()
{
	Interface* pRet = new(std::nothrow)Interface();
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

bool Interface::init()
{	
	flag = 0;
	Wflag = 0;
	Mflag = 0;
	ismusic = 1;
	stage = 0;
	tool = 0;
	char str[20] = "";


	//粒子特效
	ParticleSystemQuad* smoke = ParticleSystemQuad::create("particle/myparticle.plist");
	smoke->setScale(2.0);
	smoke->setPosition(Vec2(0, 336));
	smoke->setTag(500);
	smoke->setVisible(false);
	this->addChild(smoke, 100);

	//游戏主界面
	auto main_UI = GUIReader::getInstance()->widgetFromJsonFile("002_1/startscene_1.ExportJson");
	addChild(main_UI);
	main_UI->setTag(1001);

	auto btn1_main_UI = (Button*)main_UI->getChildByTag(2)->getChildByTag(3);
	btn1_main_UI->addClickEventListener(CC_CALLBACK_0(Interface::showchoose, this));

	auto btn2_main_UI = (Button*)main_UI->getChildByTag(2)->getChildByTag(4);
	btn2_main_UI->addClickEventListener(CC_CALLBACK_0(Interface::showhelp, this));

	auto btn3_main_UI = (Button*)main_UI->getChildByTag(2)->getChildByTag(5);
	btn3_main_UI->addClickEventListener(CC_CALLBACK_0(Interface::quit, this));

	//auto btn4_main_UI = (Button*)main_UI->getChildByTag(2)->getChildByTag(11)->getChildByTag(12);
	//btn4_main_UI->addClickEventListener(CC_CALLBACK_0(Interface::quit, this));

	//auto btn5_main_UI = (Button*)main_UI->getChildByTag(2)->getChildByTag(7)->getChildByTag(8);
	//btn5_main_UI->addClickEventListener(CC_CALLBACK_0(Interface::showranking_List, this));

	//帮助界面
	auto help_UI = GUIReader::getInstance()->widgetFromJsonFile("003_1/helpscene_1.ExportJson");
	addChild(help_UI);
	help_UI->setTag(1002);
	help_UI->setVisible(false);


	auto btn_help_UI = (Button*)help_UI->getChildByTag(5)->getChildByTag(6);
	btn_help_UI->addClickEventListener(CC_CALLBACK_0(Interface::return_mainscene, this));

	//排行榜界面
	auto list_UI = GUIReader::getInstance()->widgetFromJsonFile("004_1/004_1.ExportJson");
	addChild(list_UI);
	list_UI->setTag(1004);
	list_UI->setVisible(false);

	auto btn_list_UI = (Button*)list_UI->getChildByTag(2)->getChildByTag(3)->getChildByTag(7)->getChildByTag(8);
	btn_list_UI->addClickEventListener(CC_CALLBACK_0(Interface::return_mainscene, this));

	itoa(first_score, str, 10);
	FirstScore = Label::createWithTTF(str, "fonts/arial.ttf", 24);
	FirstScore->setPosition(Vec2(550, 475));
	FirstScore->setTag(300);
	FirstScore->setVisible(false);
	this->addChild(FirstScore, 1);

	itoa(second_score, str, 10);
	SecondScore = Label::createWithTTF(str, "fonts/arial.ttf", 24);
	SecondScore->setPosition(Vec2(550, 370));
	SecondScore->setTag(301);
	SecondScore->setVisible(false);
	this->addChild(SecondScore, 1);

	itoa(third_score, str, 10);
	ThirdScore = Label::createWithTTF(str, "fonts/arial.ttf", 24);
	ThirdScore->setPosition(Vec2(550, 255));
	ThirdScore->setTag(302);
	ThirdScore->setVisible(false);
	this->addChild(ThirdScore, 1);

	//选择关卡界面
	auto choose_UI = GUIReader::getInstance()->widgetFromJsonFile("worldUI_1/worldUI_1.ExportJson");
	addChild(choose_UI);
	choose_UI->setTag(1003);
	choose_UI->setVisible(false);

	auto btn1_choose_UI = (Button*)choose_UI->getChildByTag(2)->getChildByTag(6);
	btn1_choose_UI->addClickEventListener(CC_CALLBACK_0(Interface::choosefirst, this));

	auto btn2_choose_UI = (Button*)choose_UI->getChildByTag(2)->getChildByTag(13);
	btn2_choose_UI->addClickEventListener(CC_CALLBACK_0(Interface::choosesecond, this));

	auto btn3_choose_UI = (Button*)choose_UI->getChildByTag(2)->getChildByTag(14);
	btn3_choose_UI->addClickEventListener(CC_CALLBACK_0(Interface::choosethird, this));

	auto textView = (Text*)choose_UI->getChildByTag(2)->getChildByTag(16);
	textView->setVisible(true);
	textView->setFontSize(12);
	textView->setText(GBKToUTF8("（公元二〇一九年的某天，一种病毒悄然进入人类世界）"));

	auto doctor = (ImageView*)this->getChildByTag(1003)->getChildByTag(2)->getChildByTag(17);
	doctor->setVisible(false);
	
	auto text_bg = (ImageView*)choose_UI->getChildByTag(2)->getChildByTag(4);
	text_bg->addClickEventListener(CC_CALLBACK_0(Interface::next_stage, this));

	//auto btn4_choose_UI = (Button*)choose_UI->getChildByTag(2)->getChildByTag(6);
	//btn4_choose_UI->addClickEventListener(CC_CALLBACK_0(Interface::return_mainscene, this));

	return true;
}

void Interface::update(float delta)
{
	successOrFail();//判断输赢

	////更新所剩敌人数
	//char str[20] = "";
	//itoa(gamescene->enemy_life + gamescene->enemy_left, str, 10);
	//enemylife_score->setString(str);

	////更新玩家剩余生命数
	//itoa(gamescene->player_life, str, 10);
	//palyerlife_score->setString(str);

	////更新得分
	//itoa(player_score, str, 10);
	//score_sum->setString(str);

}

void Interface::next_stage() 
{
	auto textView = (Text*)this->getChildByTag(1003)->getChildByTag(2)->getChildByTag(16);
	auto doctor = (ImageView*)this->getChildByTag(1003)->getChildByTag(2)->getChildByTag(17);
	auto covid1 = (ImageView*)this->getChildByTag(1003)->getChildByTag(2)->getChildByTag(8);
	auto covid2 = (ImageView*)this->getChildByTag(1003)->getChildByTag(2)->getChildByTag(12);
	auto covid3 = (ImageView*)this->getChildByTag(1003)->getChildByTag(2)->getChildByTag(15);
	auto btn1_choose_UI = (Button*)this->getChildByTag(1003)->getChildByTag(2)->getChildByTag(6);
	auto btn2_choose_UI = (Button*)this->getChildByTag(1003)->getChildByTag(2)->getChildByTag(13);
	auto btn3_choose_UI = (Button*)this->getChildByTag(1003)->getChildByTag(2)->getChildByTag(14);

	textView->setVisible(true);

	SimpleAudioEngine::getInstance()->playEffect("music/next.wav", false);

	stage++;
	switch (stage)
	{
	case 1:
		doctor->setVisible(true);
		textView->setText(GBKToUTF8("你好，我是流行病学专家史蒂夫·莱里克！"));
		break;
	case 2:
		textView->setText(GBKToUTF8("来自异世界的旅行者，地球现在遇到了一些麻烦..."));
		break;
	case 3:
		textView->setText(GBKToUTF8("在中国发现了一种不明原因肺炎，而且能人传人..."));
		covid1->setVisible(true);
		break;
	case 4:
		textView->setText(GBKToUTF8("该病已知的首名病人于中国的湖北省武汉市确诊..."));
		break;
	case 5:
		textView->setText(GBKToUTF8("现在情况很紧急，我们对这种病毒没有任何了解..."));
		break;
	case 6:
		textView->setText(GBKToUTF8("请你进入病人的肺中与病毒战斗，并帮我们调查..."));
		break;
	case 7:
		textView->setText(GBKToUTF8("这个病毒，现在你可以借助我们医护资源来对抗它。"));
		btn1_choose_UI->setVisible(true);
		break;
	case 8:
		textView->setText(GBKToUTF8("好险，靠病人的自身免疫系统和全力治疗康复了。"));
		break;
	case 9:
		textView->setText(GBKToUTF8("据分析，这是一种新型的冠状病毒，是一种具有..."));
		break;
	case 10:
		textView->setText(GBKToUTF8("包膜的正链单股RNA病毒，在人群中有较强传染性。"));
		break;
	case 11:
		textView->setText(GBKToUTF8("世卫组织将其命名为SARS-CoV-2，疾病名为COVID-19。"));
		break;
	case 12:
		textView->setText(GBKToUTF8("几个月过去了，世卫组织已将其评估为「全球大流行病」。"));
		covid2->setVisible(true);
		break;
	case 13:
		textView->setText(GBKToUTF8("现在中国的疫情已经稳定了下来，能否请你到当前疫情..."));
		break;
	case 14:
		textView->setText(GBKToUTF8("最严重的欧洲地区出趟差，继续调查一下这种病毒吧。"));
		break;
	case 15:
		textView->setText(GBKToUTF8("我们发现了一种药物对症状有缓解作用，不妨试试！"));
		btn2_choose_UI->setVisible(true);
		break;
	case 16:
		textView->setText(GBKToUTF8("看来药物对症治疗也有用呢，看来疫苗的效果值得期待..."));
		break;
	case 17:
		textView->setText(GBKToUTF8("哎呀！不小心说漏嘴了..."));
		break;
	case 18:
		textView->setText(GBKToUTF8("没错，各国的研发团队已经研究出多款不同技术路线的疫苗..."));
		break;
	case 19:
		textView->setText(GBKToUTF8("很多国家也开启了紧急授权使用新冠疫苗，并全民大规模接种。"));
		break;
	case 20:
		textView->setText(GBKToUTF8("从社会层面说，新冠疫苗能够有效阻断疫情传播链..."));
		break;
	case 21:
		textView->setText(GBKToUTF8("从个人层面说，接种疫苗可以大大降低重症和死亡率..."));
		break;
	case 22:
		textView->setText(GBKToUTF8("无论对自己还是对他人都是一种保护。"));
		break;
	case 23:
		textView->setText(GBKToUTF8("截至当前，全球新冠病毒病已经累计确诊逾五亿例..."));
		covid3->setVisible(true);
		break;
	case 24:
		textView->setText(GBKToUTF8("疫情形势依然严峻，德尔塔、奥密克戎变异株纷至沓来..."));
		break;
	case 25:
		textView->setText(GBKToUTF8("但这一次我们有了疫苗，一定可以找到一条最优的道路！"));
		btn3_choose_UI->setVisible(true);
		break;
	case 26:
		textView->setText(GBKToUTF8("我们成功了！希望新冠疫情能在人类的共同努力下早日结束！"));
		break;
	case 27:
		return_mainscene();
		break;
	default:
		break;
	}
}

void Interface::gameinterface()
{
	flag = 1;
	char str[20] = "";

	//战斗界面
	auto combat_UI = GUIReader::getInstance()->widgetFromJsonFile("006_1/combat_1.ExportJson");
	addChild(combat_UI);
	combat_UI->setTag(1006);
	combat_UI->setVisible(true);

	auto btn1_combat_UI = (Button*)combat_UI->getChildByTag(4)->getChildByTag(5);
	btn1_combat_UI->addClickEventListener(CC_CALLBACK_0(Interface::tool_doc, this));

	auto btn2_combat_UI = (Button*)combat_UI->getChildByTag(4)->getChildByTag(6);
	btn2_combat_UI->addClickEventListener(CC_CALLBACK_0(Interface::tool_pill, this));
	btn2_combat_UI->setVisible(false);

	auto btn3_combat_UI = (Button*)combat_UI->getChildByTag(4)->getChildByTag(7);
	btn3_combat_UI->addClickEventListener(CC_CALLBACK_0(Interface::tool_vaccine, this));
	btn3_combat_UI->setVisible(false);

	////所剩敌人数
	//auto enemylife = Label::createWithTTF("Enemy:", "fonts/arial.ttf", 24);
	//enemylife->setPosition(Vec2(1020, 630));
	//enemylife->setTag(100);
	//enemylife->setVisible(false);
	//this->addChild(enemylife, 1);

	//itoa(gamescene->enemy_life, str, 10);
	//enemylife_score = Label::createWithTTF(str, "fonts/arial.ttf", 24);
	//enemylife_score->setPosition(Vec2(1020, 580));
	//enemylife_score->setTag(101);
	//enemylife_score->setVisible(false);
	//this->addChild(enemylife_score, 1);

	////玩家生命数
	//auto palyerlife = Label::createWithTTF("Life:", "fonts/arial.ttf", 24);
	//palyerlife->setPosition(Vec2(1020, 530));
	//palyerlife->setTag(102);
	//palyerlife->setVisible(false);
	//this->addChild(palyerlife, 1);

	//itoa(gamescene->player_life, str, 10);
	//palyerlife_score = Label::createWithTTF(str, "fonts/arial.ttf", 24);
	//palyerlife_score->setPosition(Vec2(1020, 480));
	//palyerlife_score->setTag(103);
	//palyerlife_score->setVisible(false);
	//this->addChild(palyerlife_score, 1);

	////分数
	//auto score = Label::createWithTTF("Your Score", "fonts/arial.ttf", 24);
	//score->setPosition(Vec2(1020, 430));
	//score->setTag(104);
	//score->setVisible(false);
	//this->addChild(score, 1);

	//itoa(player_score, str, 10);
	//score_sum = Label::createWithTTF(str, "fonts/arial.ttf", 24);
	//score_sum->setPosition(Vec2(1020, 380));
	//score_sum->setTag(109);
	//this->addChild(score_sum, 1);

	////重开按钮
	//auto replay = Label::createWithTTF("Replay", "fonts/arial.ttf", 24);
	//auto replayItem = MenuItemLabel::create(replay, CC_CALLBACK_0(Interface::replay, this));
	//replayItem->setPosition(Vec2(470, -225));
	//auto menu3 = Menu::create(replayItem, NULL);
	//this->addChild(menu3, 1);
	//menu3->setTag(108);
	//menu3->setVisible(false);

	////暂停按钮
	//auto pause = Label::createWithTTF("Pause", "fonts/arial.ttf", 24);
	//auto pauseItem = MenuItemLabel::create(pause, CC_CALLBACK_0(Interface::pauseScene, this));
	//pauseItem->setPosition(Vec2(470, -75));
	//auto menu2 = Menu::create(pauseItem, NULL);
	//this->addChild(menu2, 1);
	//menu2->setTag(105);
	//menu2->setVisible(false);

	////恢复按钮
	//auto resume = Label::createWithTTF("Continue", "fonts/arial.ttf", 24);
	//auto resumeItem = MenuItemLabel::create(resume, CC_CALLBACK_0(Interface::resumeScene, this));
	//resumeItem->setPosition(Vec2(470, -150));
	//auto menu1 = Menu::create(resumeItem, NULL);
	//this->addChild(menu1, 1);
	//menu1->setTag(106);
	//menu1->setVisible(false);

	////返回按钮
	//auto return_main = Label::createWithTTF("Return", "fonts/arial.ttf", 24);
	//auto returnItem = MenuItemLabel::create(return_main, CC_CALLBACK_0(Interface::game_return, this));
	//returnItem->setPosition(Vec2(470, -300));
	//auto menu = Menu::create(returnItem, NULL);
	//this->addChild(menu, 1);
	//menu->setTag(107);
	//menu->setVisible(false);

	////音乐开关按钮
	//auto music = Label::createWithTTF("Musicset", "fonts/arial.ttf", 24);
	//auto musicItem = MenuItemLabel::create(music, CC_CALLBACK_0(Interface::playmusic, this));
	//musicItem->setPosition(Vec2(470, 0));
	//auto menu8 = Menu::create(musicItem, NULL);
	//this->addChild(menu8, 1);
	//menu8->setTag(110);
	//menu8->setVisible(false);

	//成功提示
	auto successflag = Label::createWithTTF(GBKToUTF8("无伤治愈！"), "fonts/simhei.ttf", 24);
	successflag->setScale(3);
	successflag->setColor(Color3B(255, 255, 255));
	successflag->setPosition(Vec2(480, 400));
	successflag->setTag(200);
	successflag->setVisible(false);
	this->addChild(successflag, 20);

	auto success_return_main = Label::createWithTTF(GBKToUTF8("返回剧情"), "fonts/simhei.ttf", 24);
	auto successreturnItem = MenuItemLabel::create(success_return_main, CC_CALLBACK_0(Interface::game_return, this));
	successreturnItem->setPosition(Vec2(50, -50));
	successreturnItem->setScale(2);
	successreturnItem->setColor(Color3B(255, 255, 255));
	auto menu6 = Menu::create(successreturnItem, NULL);
	this->addChild(menu6, 1);
	menu6->setTag(204);
	menu6->setVisible(false);

	auto success_next = Label::createWithTTF(GBKToUTF8("下一关卡"), "fonts/simhei.ttf", 24);
	auto nextItem = MenuItemLabel::create(success_next, CC_CALLBACK_0(Interface::game_next, this));
	nextItem->setPosition(Vec2(-170, -50));
	nextItem->setScale(2);
	nextItem->setColor(Color3B(255, 255, 255));
	auto menu7 = Menu::create(nextItem, NULL);
	this->addChild(menu7, 1);
	menu7->setTag(205);
	menu7->setVisible(false);

	//失败提示
	auto failflag1 = Label::createWithTTF(GBKToUTF8("病毒攻陷！"), "fonts/simhei.ttf", 24);
	failflag1->setScale(3);
	failflag1->setColor(Color3B(255, 255, 255));
	failflag1->setPosition(Vec2(480, 400));
	failflag1->setTag(201);
	failflag1->setVisible(false);
	this->addChild(failflag1, 20);
	
	auto fail_replay = Label::createWithTTF(GBKToUTF8("再试一次"), "fonts/simhei.ttf", 24);
	auto failreplayItem = MenuItemLabel::create(fail_replay, CC_CALLBACK_0(Interface::replay, this));
	failreplayItem->setPosition(Vec2(-170, -50));
	failreplayItem->setScale(2);
	failreplayItem->setColor(Color3B(255, 255, 255));
	auto menu4 = Menu::create(failreplayItem, NULL);
	this->addChild(menu4, 1);
	menu4->setTag(202);
	menu4->setVisible(false);

	auto fail_return_main = Label::createWithTTF(GBKToUTF8("返回剧情"), "fonts/simhei.ttf", 24);
	auto failreturnItem = MenuItemLabel::create(fail_return_main, CC_CALLBACK_0(Interface::game_return, this));
	failreturnItem->setPosition(Vec2(50, -50));
	failreturnItem->setScale(2);
	failreturnItem->setColor(Color3B(255, 255, 255));
	auto menu5 = Menu::create(failreturnItem, NULL);
	this->addChild(menu5, 1);
	menu5->setTag(203);
	menu5->setVisible(false);

	auto success_panel = GUIReader::getInstance()->widgetFromJsonFile("combat_win_1/combat_win_1.ExportJson");
	addChild(success_panel);
	success_panel->setTag(1070);
	success_panel->setVisible(false);

	auto success_return_bt = (Button*)success_panel->getChildByTag(2)->getChildByTag(4);
	success_return_bt->addClickEventListener(CC_CALLBACK_0(Interface::game_return, this));

	auto fail_panel = GUIReader::getInstance()->widgetFromJsonFile("combat_lose_1/combat_lose_1.ExportJson");
	addChild(fail_panel);
	fail_panel->setTag(1080);
	fail_panel->setVisible(false);

	auto fail_replay_bt = (Button*)fail_panel->getChildByTag(2)->getChildByTag(3);
	fail_replay_bt->addClickEventListener(CC_CALLBACK_0(Interface::replay, this));

	auto fail_return_bt = (Button*)fail_panel->getChildByTag(2)->getChildByTag(4);
	fail_return_bt->addClickEventListener(CC_CALLBACK_0(Interface::game_return, this));

}

//更新排行榜
void Interface::updateranking_List()
{
	if (player_score > first_score)
	{		
		third_score = second_score;
		second_score = first_score;
		first_score = player_score;
	}
	else if (player_score > second_score&&player_score < first_score)
	{
		third_score = second_score;
		second_score = player_score;
	}
	else if (player_score > third_score&&player_score < second_score)
	{
		third_score = player_score;
	}
	char str[20] = "";
	itoa(first_score, str, 10);
	FirstScore->setString(str);

	itoa(second_score, str, 10);
	SecondScore->setString(str);

	itoa(third_score, str, 10);
	ThirdScore->setString(str);
}

//重开函数
void Interface::replay()
{
	//Mflag = 0;
	//this->getChildByTag(201)->setVisible(false);
	//this->getChildByTag(202)->setVisible(false);
	//this->getChildByTag(203)->setVisible(false);
	this->getChildByTag(1080)->setVisible(false);
	//this->getChildByTag(500)->setVisible(false);
	//updateranking_List();
	//player_score = 0;
	gamescene->removeFromParent();
	//gamescene = BattleScene::create(level);
	//this->getParent()->addChild(gamescene);
	if (level == 1) choosefirst();
	if (level == 2) choosesecond();
	if (level == 3) choosethird();
}

//进入下一关
void Interface::game_next()
{
	Mflag = 0;
	if (level < 3)
	{
		level++;
		//this->getChildByTag(200)->setVisible(false);
		//this->getChildByTag(204)->setVisible(false);
		//this->getChildByTag(205)->setVisible(false);
		this->getChildByTag(1070)->setVisible(false);
		gamescene->removeFromParent();
		gamescene = BattleScene::create(level);
		this->getParent()->addChild(gamescene);
	}
	else
		game_return();
}

//返回主界面
void Interface::return_mainscene()
{
	this->getChildByTag(300)->setVisible(false);
	this->getChildByTag(301)->setVisible(false);
	this->getChildByTag(302)->setVisible(false);
	this->getChildByTag(1004)->setVisible(false);
	this->getChildByTag(1003)->setVisible(false);
	this->getChildByTag(1002)->setVisible(false);
	this->getChildByTag(1001)->setVisible(true);
}

//返回
void Interface::game_return()
{
	//level = 1;
	updateranking_List();
	player_score = 0;
	Wflag = 0;
	//for (int i = 100; i < 111; i++)
		//this->getChildByTag(i)->setVisible(false);	
	//this->getChildByTag(200)->setVisible(false);
	//this->getChildByTag(201)->setVisible(false);
	//this->getChildByTag(202)->setVisible(false);
	//this->getChildByTag(203)->setVisible(false);
	this->getChildByTag(1070)->setVisible(false);
	this->getChildByTag(1080)->setVisible(false);
	//this->getChildByTag(204)->setVisible(false);
	//this->getChildByTag(205)->setVisible(false);
	this->getChildByTag(500)->setVisible(false);
	this->getChildByTag(1006)->getChildByTag(4)->setVisible(false);
	this->getChildByTag(1001)->setVisible(true);	
	gamescene->removeFromParent();
	next_stage();
	auto choose_UI = this->getChildByTag(1003);
	choose_UI->setVisible(true);
}

//显示排行榜界面
void Interface::showranking_List()
{
	this->getChildByTag(300)->setVisible(true);
	this->getChildByTag(301)->setVisible(true);
	this->getChildByTag(302)->setVisible(true);
	this->getChildByTag(1001)->setVisible(false);
	this->getChildByTag(1004)->setVisible(true);
}

//显示选择关卡界面
void Interface::showchoose()
{
	this->getChildByTag(1001)->setVisible(false);
	this->getChildByTag(1003)->setVisible(true);
}

//选择第一关
void Interface::choosefirst()
{
	Mflag = 0;
	level = 1;
	this->getChildByTag(1001)->setVisible(false);
	auto bt1 = (Button*)this->getChildByTag(1003)->getChildByTag(2)->getChildByTag(6);
	bt1->setBright(false);
	bt1->setEnabled(false);
	this->getChildByTag(1003)->setVisible(false);
	this->getChildByTag(500)->setVisible(false);
	gamescene = BattleScene::create(level);
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/level1.mp3", true);
	Wflag = 1;
	this->getParent()->addChild(gamescene);
	if (flag == 0)
		gameinterface();
	else
	{
		this->getChildByTag(1006)->getChildByTag(4)->setVisible(true);
		this->getChildByTag(1006)->getChildByTag(4)->getChildByTag(5)->setVisible(true);
		this->getChildByTag(1006)->getChildByTag(4)->getChildByTag(6)->setVisible(false);
		this->getChildByTag(1006)->getChildByTag(4)->getChildByTag(7)->setVisible(false);
	}
	this->scheduleUpdate();
}

//选择第二关
void Interface::choosesecond()
{
	Mflag = 0;
	level = 2;
	this->getChildByTag(1001)->setVisible(false);
	auto bt2 = (Button*)this->getChildByTag(1003)->getChildByTag(2)->getChildByTag(13);
	bt2->setBright(false);
	bt2->setEnabled(false);
	this->getChildByTag(1003)->setVisible(false);
	this->getChildByTag(500)->setVisible(false);
	gamescene = BattleScene::create(level);
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/level2.mp3", true);
	Wflag = 1;
	this->getParent()->addChild(gamescene);
	if (flag == 0)
		gameinterface();
	else
	{
		this->getChildByTag(1006)->getChildByTag(4)->setVisible(true);
		this->getChildByTag(1006)->getChildByTag(4)->getChildByTag(5)->setVisible(true);
		this->getChildByTag(1006)->getChildByTag(4)->getChildByTag(6)->setVisible(true);
		this->getChildByTag(1006)->getChildByTag(4)->getChildByTag(7)->setVisible(false);
	}
	this->scheduleUpdate();
}

//选择第三关
void Interface::choosethird()
{
	Mflag = 0;
	level = 3;
	this->getChildByTag(1001)->setVisible(false);
	auto bt3 = (Button*)this->getChildByTag(1003)->getChildByTag(2)->getChildByTag(14);
	bt3->setBright(false);
	bt3->setEnabled(false);
	this->getChildByTag(1003)->setVisible(false);
	this->getChildByTag(500)->setVisible(false);
	gamescene = BattleScene::create(level);
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/level3.mp3", true);
	Wflag = 1;
	this->getParent()->addChild(gamescene);
	if (flag == 0)
		gameinterface();
	else
	{
		this->getChildByTag(1006)->getChildByTag(4)->setVisible(true);
		this->getChildByTag(1006)->getChildByTag(4)->getChildByTag(5)->setVisible(true);
		this->getChildByTag(1006)->getChildByTag(4)->getChildByTag(6)->setVisible(true);
		this->getChildByTag(1006)->getChildByTag(4)->getChildByTag(7)->setVisible(true);
	}
	this->scheduleUpdate();
}

//显示帮助界面
void Interface::showhelp()
{
	this->getChildByTag(1001)->setVisible(false);
	this->getChildByTag(1002)->setVisible(true);
}

//退出游戏
void Interface::quit()
{
	Director::getInstance()->end();
}

//开始游戏
void Interface::start()
{
	Mflag = 0;
	this->getChildByTag(1001)->setVisible(false);
	this->getChildByTag(1003)->setVisible(true);
	gamescene = BattleScene::create(level);
	this->getParent()->addChild(gamescene);
	if (flag == 0)
		gameinterface();
	else
	{
		for (int i = 100; i < 111; i++)
			this->getChildByTag(i)->setVisible(true);
	}
	Wflag = 1;
	this->scheduleUpdate();
}

//暂停
void Interface::pauseScene(Ref* pSender)
{
	auto vector = gamescene->getChildren();
	for (auto i = vector.begin(); i != vector.end(); ++i)
	{
		Node *node = *i;
		auto vector_ = node->getChildren();
		for (auto j = vector_.begin(); j != vector_.end(); ++j)
		{
			(*j)->pause();
		}
		(*i)->pause();
	}
	gamescene->pause();
}

void Interface::pausescene()
{
	std::stack<Node*> node_stack;
	node_stack.push(gamescene);
	while(!node_stack.empty())
	{
		Node* node = node_stack.top();
		node_stack.pop();
		auto vector = node->getChildren();
		for (auto i = vector.begin(); i != vector.end(); ++i)
			node_stack.push((*i));
		node->pause();
	}
}

//恢复
void Interface::resumeScene(Ref* pSender)
{
	std::stack<Node*> node_stack;
	node_stack.push(gamescene);
	while(!node_stack.empty())
	{
		Node* node = node_stack.top();
		node_stack.pop();
		auto vector = node->getChildren();
		for (auto i = vector.begin(); i != vector.end(); ++i)
			node_stack.push((*i));
		node->resume();
	}
}

//成败判定
void Interface::successOrFail()
{
	if ((gamescene->player_life <= 0 || gamescene->guard_life <= 0) && Wflag)
	{
		pausescene();
		//this->getChildByTag(201)->setVisible(true);
		//this->getChildByTag(202)->setVisible(true);
		//this->getChildByTag(203)->setVisible(true);
		this->getChildByTag(1080)->setVisible(true);
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		if (Mflag == 0)
		{
			SimpleAudioEngine::getInstance()->playEffect("music/lose.wav");
			Mflag = 1;
		}
	}
	else if ((gamescene->enemy_left + gamescene->enemy_life )<= 0 && Wflag)
	{
		pausescene();
		//this->getChildByTag(200)->setVisible(true);
		//this->getChildByTag(204)->setVisible(true);
		//this->getChildByTag(205)->setVisible(true);
		this->getChildByTag(1070)->setVisible(true);
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		if (Mflag == 0)
		{
			SimpleAudioEngine::getInstance()->playEffect("music/win.wav");
			Mflag = 1;
		}
	}
}

//音乐开关
void Interface::playmusic()
{
	if (ismusic == 0)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("music/background.wav");
		ismusic = 1;
	}
	else
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		ismusic = 0;
	}
}

void Interface::tool_doc()
{
	this->getChildByTag(500)->setVisible(true);
	this->getChildByTag(1006)->getChildByTag(4)->getChildByTag(5)->setVisible(false);
}

void Interface::tool_pill()
{
	tool = 2;
	gamescene->player_life++;
	this->getChildByTag(1006)->getChildByTag(4)->getChildByTag(6)->setVisible(false);
}

void Interface::tool_vaccine()
{
	tool = 3;
	this->getChildByTag(1006)->getChildByTag(4)->getChildByTag(7)->setVisible(false);
}