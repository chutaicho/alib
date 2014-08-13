//
//  Copyright (c) 2014 Takashi Aoki
//
//  Released under the MIT license
//  http://opensource.org/licenses/mit-license.php
//

#pragma once
#include <iostream>
#include <iterator>
#include "ofMain.h"

namespace at
{
    enum TextBoxBackGround { OF_TEXT_BG_NONE, OF_TEXT_BG_AREA, OF_TEXT_BG_BOX };
    
    static const float TEXT_PADDING_H = 1.20;//15;//1.15; // box padding y - relative
    static const float TEXT_PADDING_L = 15; // box padding x - absolute
    static const float TEXT_PADDING_R = 15;
    static const float TEXT_SPACE_SIZE = .5;
    static const float TEXT_LINE_SPACE_BGAREA = -2.0;
    static const float TEXT_LINE_SPACE_BGNONE = -0.2;
    
    static const int TEXT_MAX_WORDS = 50;
    
    class wordBlock
    {
    public:
        std::string rawWord;
        float width;
        float height;
        float lineHeight;
        ofColor color;
    };
    class lineBlock
    {
    public:
        std::vector<int>   wordsID;
        std::string words;
        float   width;
        float   height;
    };
    class TextBox
    {
    public:
        TextBox();
        ~TextBox();
        
        std::string rawText;
        wordBlock       blankSpaceWord;
        float           scale;
        
        std::vector<wordBlock>   words;
        std::vector<lineBlock>   lines;
        
        void setup(ofTrueTypeFont* font/*,int maxWords=0*/);
        
        void setText(std::string str);
        void setColor(int textHex, int bgHex=0xFFFFFF);
        void setBgMode(TextBoxBackGround mode);
        
        int  setBoxWidth(float lineWidth); // Returns the number of lines it formed.
        void setLineHeight(float lineHeight);
        
        void draw(float x, float y);
        void drawText(float x, float y);
        void drawBG(float x, float y);
        
        void clear();
        
        int   getNumWords();
        float getWidth();
        float getHeight();
        int   getPadding();
        float getLineSpace();
        
        TextBoxBackGround bgMode;
        
    private:
        ofTrueTypeFont*  _defaultFont;
        
        void loadWords();
        
        float getWidthOfWords();
        int   getLinedWords();
        
        ofColor _bgColor;
        ofColor _textColor;
        int _maxWords;
        
        float _lineHeight;
        float _padding;
        float _lineSpace;
    };
};

