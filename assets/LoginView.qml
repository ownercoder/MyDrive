import bb.cascades 1.3

Page {
    Container {
        layout: DockLayout { }
        Button {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            text: qsTr("Sign In")
            onClicked: {
                loginWebViewSheet.loginUrl = app.getLoginUrl();
                loginWebViewSheet.open();
            }
        }
    }
    attachedObjects: [
        LoginWebViewSheet {
            id: loginWebViewSheet
        }
    ]
    function onAuthenticationReceived() {
        loginWebViewSheet.close();
    }
}
