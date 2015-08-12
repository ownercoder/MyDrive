#ifndef WEBIMAGEVIEW_H_
#define WEBIMAGEVIEW_H_

#include <bb/cascades/ImageView>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QUrl>
using namespace bb::cascades;

class WebImageView: public bb::cascades::ImageView {
	Q_OBJECT
	Q_PROPERTY (QUrl url READ url WRITE setUrl NOTIFY urlChanged)
	Q_PROPERTY (QUrl mimeImage READ mimeImage WRITE setMimeImage)
	Q_PROPERTY (float loading READ loading NOTIFY loadingChanged)
	Q_PROPERTY (QVariant filterColor READ filterColor WRITE setFilterColor)

public:
	WebImageView();
	const QUrl& url() const;
	const QUrl& mimeImage() const;
	double loading() const;

	public Q_SLOTS:
	void setMimeImage(const QUrl& url);
	void setUrl(const QUrl& url);
    void clearCache();

	private Q_SLOTS:
	void imageLoaded();
	void dowloadProgressed(qint64,qint64);

	signals:
	void urlChanged();
	void loadingChanged();


private:
	static QNetworkAccessManager * mNetManager;
	static QNetworkDiskCache * mNetworkDiskCache;
	QUrl mUrl;
	QUrl mimeImageUrl;
	float mLoading;

	bool isARedirectedUrl(QNetworkReply *reply);
	void setURLToRedirectedUrl(QNetworkReply *reply);
};

#endif /* WEBIMAGEVIEW_H_ */
