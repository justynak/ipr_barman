#ifndef THEME_H
#define THEME_H

#include <QFont>
#include <QWidget>

// Companion to theme.qss for the one thing Qt stylesheets cannot express:
// letter-spacing on the display headings.
namespace Theme
{
    inline void LetterSpace(QWidget* widget, qreal pixels)
    {
        QFont font = widget->font();
        font.setLetterSpacing(QFont::AbsoluteSpacing, pixels);
        widget->setFont(font);
    }
}

#endif // THEME_H
