//
// Copyright Petrostreamz 2008
//

#ifndef ICONSET_H
#define ICONSET_H
#define EmbIcon IconSet::icon
#define EmbPixmap IconSet::pixmap

class QIcon;

/*!
 The class provides functionality
 for accessing icons embedded into Pipe-It.
 Icons are cached and loaded only once on first request.
 */
class IconSet {
public:
    static QIcon icon(QString);
    static QPixmap pixmap(QString);

private:
    class Private;
    Private * d;
};

#endif // ICONSET_H
