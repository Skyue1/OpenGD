/*************************************************************************
    OpenGD - Open source Geometry Dash.
    Copyright (C) 2023  OpenGD Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License    
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*************************************************************************/

#include "TutorialLayer.h"
#include <MenuItemSpriteExtra.h>
#include "UTF8.h"
#include <GameToolbox/getTextureString.h>
#include "ui/UIScale9Sprite.h"
#include "ButtonSprite.h"
#include "2d/ActionEase.h"

USING_NS_AX;

TutorialLayer* TutorialLayer::create() {
    auto layer = new TutorialLayer();
    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    } else {
        delete layer;
        return nullptr;
    }
}

bool TutorialLayer::init() {
    if (!PopupLayer::init()) return false;

    std::string bigFontStr = GameToolbox::getTextureString("bigFont.fnt");

    auto director = Director::getInstance();
    const auto& winSize = director->getWinSize();

    layer1 = Layer::create();
    addChild(layer1);

    auto bg = ui::Scale9Sprite::create(GameToolbox::getTextureString("GJ_square01.png"));
    bg->setContentSize({ 420.0f, 260.0f });
    bg->setPosition({ 294.5f, 163.0f });
    layer1->addChild(bg);

    auto label = Label::createWithBMFont(bigFontStr, "How to play");
    label->setPosition({ 282.5f, 265.0f });
    if (label->getContentSize().width > 300.0f) {
        label->setScaleX(300.0f / label->getContentSize().width);
    }
    layer1->addChild(label);

    tap_label = Label::createWithBMFont(bigFontStr, "Tap the screen to jump.\n");
    tap_label->setPosition({ 278.6f, 222.1f });
    tap_label->setScale(0.7f);
    layer1->addChild(tap_label);

    hold_label = Label::createWithBMFont(bigFontStr, "Hold down to keep jumping.");
    hold_label->setPosition({ 294.8f, 193.2f });
    hold_label->setScale(0.8f);
    layer1->addChild(hold_label);

    auto imageSprite = Sprite::create(GameToolbox::getTextureString("tutorial_01.png"));
    imageSprite->setScale(0.95f);
    imageSprite->setPosition({ 239.0f, 84.0f });
    layer1->addChild(imageSprite);

    auto nextButtonSprite = ButtonSprite::create("Next");
    auto nextButton = MenuItemSpriteExtra::create(nextButtonSprite, [&](Node*) { this->onNext(); });
    auto nextButtonMenu = Menu::createWithItem(nextButton);
    nextButtonMenu->setPosition({ 437.25f, 84.0f });
    nextButtonMenu->setTag(150);
    layer1->addChild(nextButtonMenu);

    currentTutorialIndex = 1;

    auto closeButtonSprite = Sprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    auto closeButton = MenuItemSpriteExtra::create(closeButtonSprite, [&](Node*) { this->close(); });
    auto closeButtonMenu = Menu::createWithItem(closeButton);
    closeButtonMenu->setPosition({ 93.0f, 285.75f });
    layer1->addChild(closeButtonMenu);

    layer1->setScale(0.f);
    layer1->runAction(EaseElasticOut::create(ScaleTo::create(0.5f, 1.0f), 0.6f));

    return true;
}

void TutorialLayer::onNext() {
    currentTutorialIndex++;
    std::string fileName = "tutorial_0" + std::to_string(currentTutorialIndex) + ".png";
    layer1->removeChildByName("tutorial_01.png");

    if (fileName == "tutorial_02.png") {
        tap_label->setString("Hold to fly up.\n");
        hold_label->setString("Release to fly down.");
    } else if (fileName == "tutorial_03.png") {
        tap_label->setString("   \nYou can enter practice mode from\n   the pause menu.\n");
        tap_label->setScale(0.6f);
        tap_label->setAlignment(TextHAlignment::CENTER);

        hold_label->setString("  \nPractice mode lets you place\n checkpoints.");
        hold_label->setScale(0.6f);
        hold_label->setAlignment(TextHAlignment::CENTER);
    } else if (fileName == "tutorial_04.png") {
        tap_label->setString("   \nYou can place checkpoints manually, or\n use the auto-checkpoint feature.\n");
        tap_label->setScale(0.55f);
        tap_label->setAlignment(TextHAlignment::CENTER);
        tap_label->setPosition({ 291.6f, 222.1f });

        hold_label->setString("  \nTap the delete button to remove your\n last checkpoint.");
        hold_label->setScale(0.6f);
        hold_label->setAlignment(TextHAlignment::CENTER);
    } else if (fileName == "tutorial_05.png") {
        tap_label->setString("   \nJump rings activate when you are on\n top of them.\n");
        tap_label->setScale(0.6f);
        tap_label->setAlignment(TextHAlignment::CENTER);

        hold_label->setString("  \nTap while touching a ring to perform\n a ring jump.");
        hold_label->setScale(0.6f);
        hold_label->setAlignment(TextHAlignment::CENTER);

        layer1->removeChildByTag(150, true);
        auto exitButtonSprite = ButtonSprite::create("Exit");
        auto exitButton = MenuItemSpriteExtra::create(exitButtonSprite, [&](Node*) { this->close(); });
        auto exitButtonMenu = Menu::createWithItem(exitButton);
        exitButtonMenu->setPosition({ 437.25f, 84.0f });
        layer1->addChild(exitButtonMenu);
    }

    if (layer1 && currentTutorialIndex <= 5) {
        auto imageSprite = Sprite::create(GameToolbox::getTextureString(fileName));
        imageSprite->setScale(0.95f);
        imageSprite->setPosition({ 239.0f, 84.0f });
        layer1->addChild(imageSprite);
    }
}
