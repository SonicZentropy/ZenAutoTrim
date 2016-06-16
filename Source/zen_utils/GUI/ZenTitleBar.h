/*==============================================================================
//  ZenTitleBar.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2016/05/19
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Zen top header/title bar Component
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#pragma once
#ifndef ZEN_TITLEBAR_H_INCLUDED
#define ZEN_TITLEBAR_H_INCLUDED

#include <JuceHeader.h>

class ZenTitleBar : public Component,
	public ImageButton::Listener
{
public:

	ZenTitleBar(const String& compName, Component* inOwner);
	ZenTitleBar(const String& compName, Component* inOwner, const Image& inTitleTextImage);

	void init();
	void addEnableListener(Listener* theListener);
	~ZenTitleBar();

	//void initColors();
	void loadImages();

	void paint(Graphics&) override;

	const Image& getBackgroundImage() const { return backgroundImg; }

	void buttonClicked(Button*) override;
	Button* getEnableBtn() const { return enableBtn; }
	

protected:
	Image backgroundImg;
	Image titleTextImage;
	Image enableOnImg, enableOffImg;
	ScopedPointer<ImageButton> enableBtn;
	
	//ZenEnableButton enableButton;
	//ZenMenuButton menuButton;

};

#endif // ZEN_TITLEBAR_H_INCLUDED