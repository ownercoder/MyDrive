import bb.cascades 1.3

Sheet {
    id: webPageSheet
    property alias loginUrl: loginWebView.url
    content: Page {
        titleBar: TitleBar {
            title: qsTr("Access request")
            dismissAction: ActionItem {
                onTriggered: {
                    webPageSheet.close();
                }
                title: qsTr("close")
            }
        
        }
        Container {
            background: Color.White
            ScrollView {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                scrollViewProperties {
                    scrollMode: ScrollMode.Vertical
                }
                WebView {
                    objectName: "loginWebView"
                    id: loginWebView
                    onLoadProgressChanged: {
                        progressIndicator.value = loadProgress;
                    }
                    onLoadingChanged: {
                        if (loadRequest.status == WebLoadStatus.Started) {
                            progressIndicator.visible = true;
                        }
                        if (loadRequest.status == WebLoadStatus.Succeeded) {
                            progressIndicator.visible = false;
                            progressIndicator.value = 0;
                        }
                    }
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                }
            }
            attachedObjects: LayoutUpdateHandler {
                onLayoutFrameChanged: {
                    loginWebView.preferredHeight = layoutFrame.height;
                }
            }
            layout: StackLayout {
                orientation: LayoutOrientation.TopToBottom
            
            }
            ProgressIndicator {
                visible: true
                id: progressIndicator
                horizontalAlignment: HorizontalAlignment.Fill
                state: ProgressIndicatorState.Progress
                toValue: 100.0
                value: 0.0
                verticalAlignment: VerticalAlignment.Bottom
                implicitLayoutAnimationsEnabled: false
            }
        }
    }
}