#include "aTextBox.h"

namespace at
{   
#pragma mark -
#pragma mark constructor & destructor

    TextBox::TextBox()
    {
        scale = 1.0f;
        bgMode = OF_TEXT_BG_NONE;

        _lineHeight = 0.f;
        _padding = 0.f;
        
        _bgColor.set(100, 100, 100);
        _textColor.set(255, 255, 255);
    }
    TextBox::~TextBox()
    {
        
    }
    
#pragma mark -
#pragma mark public methods - set & get
    
    void TextBox::setText(std::string str)
    {
        rawText = str;
        loadWords();
        
        _padding = (_defaultFont->getLineHeight()*TEXT_PADDING_H) - _defaultFont->getLineHeight();
    }
    void TextBox::setColor(int textHex, int bgHex)
    {
        _bgColor.setHex(bgHex);
        _textColor.setHex(textHex);
    }
    void TextBox::setBgMode(TextBoxBackGround mode)
    {
        bgMode = mode;
    }
    void TextBox::clear()
    {
        
    }
    int TextBox::getLinedWords()
    {
        return 0;
    }
    float TextBox::getWidthOfWords()
    {
        return 0;   
    }
    int TextBox::getPadding()
    {
        return (int)_padding;
    }
    float TextBox::getWidth()
    {
        return 0;
    }
    float TextBox::getLineSpace()
    {
        return _lineSpace;
    }
    float TextBox::getHeight()
    {
        float h = (bgMode == OF_TEXT_BG_AREA)? _defaultFont->getLineHeight()+(_padding*2)+_lineSpace : _defaultFont->getLineHeight()+_lineSpace;
        return (words.size() > 0)? lines.size() * h : 0.f;
    }
    
#pragma mark -
#pragma mark public methods
    
    void TextBox::setup(ofTrueTypeFont* font/*,int maxWords*/)
    {
        _maxWords = TEXT_MAX_WORDS;
        
        _defaultFont = font;
        _defaultFont->setSpaceSize(TEXT_SPACE_SIZE);
        
        //Set up the blank space word
        blankSpaceWord.rawWord = " ";
        blankSpaceWord.width   = _defaultFont->stringWidth ("x");
        blankSpaceWord.height  = _defaultFont->stringHeight("i");
        blankSpaceWord.color.r = blankSpaceWord.color.g = blankSpaceWord.color.b = 255;
        
        _padding = (blankSpaceWord.height*TEXT_PADDING_H) - blankSpaceWord.height;
    }
    
    void TextBox::draw(float x, float y)
    {    
        drawText(x, y);
    }
    void TextBox::drawBG(float x, float y)
    {
        _lineSpace = (bgMode == OF_TEXT_BG_AREA)? _padding*TEXT_LINE_SPACE_BGAREA : _padding*TEXT_LINE_SPACE_BGNONE;
        
        float py = 0;
        float wx = 0;
        
        if (words.size() > 0)
        {
            int i = 0;
            int s = lines.size();
            
            // draw background
            if(bgMode == OF_TEXT_BG_AREA)
            {
                _lineHeight = _defaultFont->getLineHeight()+(_padding*2);
                
                while (i<s)
                {
                    wx = _defaultFont->stringWidth(lines[i].words)+(TEXT_PADDING_L+TEXT_PADDING_R);
                    py = y + ((_lineHeight+_lineSpace) * i);
                    
                    //ofNoFill();
                    ofSetColor(_bgColor);
                    ofRect(0, py, wx, _lineHeight);
                    
                    i++;
                }
                
                i = 0;
            }
            else
            {
                _lineHeight = _defaultFont->getLineHeight();
            }
        }
    }
    void TextBox::drawText(float x, float y)
    {
        float py = 0;
        float wx = 0;
        
        if (words.size() > 0)
        {
            int i = 0;
            int s = lines.size();
            //ofFill();
            // draw text
            while (i<s)
            {
                std::string strline = lines[i].words;
                
                ofSetColor(_textColor);
                
                if(bgMode == OF_TEXT_BG_AREA) // adjust the line position when it has backgrounds
                {
                    py = y + ((_lineHeight+_lineSpace) * (i+1));
                    _defaultFont->drawString(strline, floorf(TEXT_PADDING_L), floorf(py));
                }
                else
                {
                    py = y + ((_lineHeight+_lineSpace) * (i+1));
                    _defaultFont->drawString(strline, 0.f, floorf(py));
                }
                
                i++;
            }
        }
    }
    int TextBox::setBoxWidth(float lineWidth)
    {
        if (words.size() > 0)
        {
            lines.clear();
            lineBlock tmpLine;
            
            float runningWidth = 0.0f;
            bool newLine = true;
            int activeLine = 0;
            
            for(int i=0;i < words.size(); i++)
            {
                runningWidth += words[i].width;
                
                if (runningWidth <= lineWidth)
                {
                    newLine = false;
                }
                else
                {
                    newLine = true;
                    
                    // Trim the leading spaces.
                    if(tmpLine.words.compare(0, 1," ")==0) tmpLine.words.erase(0,1);
                    
                    // ignore if the line has only blank.
                    if(tmpLine.words.size() > 0) lines.push_back(tmpLine);
                    tmpLine.words.clear();
                    runningWidth = 0.0f + words[i].width;
                    activeLine++;
                }
                
                tmpLine.words += words[i].rawWord;
                //std::cout << ">>>" << words[i].rawWord << "<<<" <<std::endl;
            }
            
            // Trim the leading spaces.
            if(tmpLine.words.compare(0, 1," ")==0) tmpLine.words.erase(0,1);
            
            //std::cout << "(2) words = " << tmpLine.words << std::endl;
            
            // Push in the final line & Ignore if the line has only blank.
            if(tmpLine.words.size() > 0) lines.push_back(tmpLine);
        }
        
        return lines.size();
    }
    
#pragma mark -
#pragma mark private methods
    
    void TextBox::loadWords()
    {
        wordBlock tmpWord;
        istringstream iss(rawText);
        
        std::vector<std::string> tokens;
        copy(istream_iterator<std::string>(iss), istream_iterator<std::string>(),back_inserter<std::vector<std::string> >(tokens));
        words.clear();
        
        for(int i=0;i < tokens.size(); i++)
        {
            tmpWord.rawWord = tokens.at(i);
            tmpWord.width   = _defaultFont->stringWidth(tmpWord.rawWord);
            tmpWord.height  = _defaultFont->stringHeight(tmpWord.rawWord);
            tmpWord.color.r = tmpWord.color.g = tmpWord.color.b = 150;
            
            if(i>_maxWords)
            {
                tmpWord.rawWord += "...";
                words.push_back(tmpWord);
                break;
            }
            else words.push_back(tmpWord);            
            
            //add spaces into the words vector if it is not the last word.
            if (i != tokens.size()) words.push_back(blankSpaceWord);
        }
    }
} // end of namespace
