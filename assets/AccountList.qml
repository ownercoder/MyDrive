import bb.cascades 1.3
import bb.device 1.3
import my.library 1.0
import com.slytech.accountdatamodel 1.0
import com.slytech.sessionwrapper 1.0

Container {
    id: listViewContainer
    property alias dataModel: accountList.dataModel
    property alias activityState: activityIndicator.running
    
    signal accountChanged()
    
    layout: DockLayout {
    
    }
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill
    ListView {
        id: accountList
        dataModel: accountDataModel
        listItemComponents: [
            ListItemComponent {
                type: "user"
                Container {
                    preferredHeight: displayInfoUser.pixelSize.height
                    preferredWidth: displayInfoUser.pixelSize.width
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    Container {
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Center
                        Container {
                            layout: AbsoluteLayout {
                            }
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                            WebImageView {
                                url: ListItemData.picture480
                                preferredWidth: ui.du(30)
                                preferredHeight: ui.du(30)
                            }
                            ImageView {
                                imageSource: "asset:///images/round-avatar.png"
                                preferredHeight: ui.du(30)
                                preferredWidth: ui.du(30)
                            }
                        } // end user avatar
                        Container {
                            layout: GridLayout {
                                columnCount: 1
                            }
                            horizontalAlignment: HorizontalAlignment.Center
                            Label {
                                text: ListItemData.displayName
                                textStyle.fontWeight: FontWeight.Bold
                                verticalAlignment: VerticalAlignment.Center
                                horizontalAlignment: HorizontalAlignment.Center
                            }
                            Label {
                                text: ListItemData.emailAddress
                                textStyle.color: Color.create("#ff4e4e4e")
                                verticalAlignment: VerticalAlignment.Center
                                horizontalAlignment: HorizontalAlignment.Center
                            }
                        }
                    }
                    attachedObjects: [
                        DisplayInfo {
                            id: displayInfoUser
                        }
                    ]
                    layout: DockLayout {}
                }
            },
            ListItemComponent {
                type: "add"
                Container {
                    preferredHeight: displayInfoAdd.pixelSize.height
                    preferredWidth: displayInfoAdd.pixelSize.width
                    Container {
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Center
                        layout: GridLayout {
                            columnCount: 1
                        }
                        ImageView {
                            imageSource: "asset:///images/add-account.png"
                            preferredHeight: ui.du(30)
                            preferredWidth: ui.du(30)
                            horizontalAlignment: HorizontalAlignment.Center
                        }
                        Label {
                            text: qsTr("Add new account")
                            textStyle.fontWeight: FontWeight.Bold
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                        }                        
                    }
                    attachedObjects: [
                        DisplayInfo {
                            id: displayInfoAdd
                        }
                    ]
                    layout: DockLayout {

                    }
                }
            }
        ]
        layout: FlowListLayout {
            orientation: LayoutOrientation.LeftToRight
        
        }
        snapMode: SnapMode.LeadingEdge
        flickMode: FlickMode.None
        visible: false
        
        function itemType(data, indexPath) {
            if (typeof accountList.dataModel.data(indexPath).addAccount != "undefined" && accountList.dataModel.data(indexPath).addAccount) {
                return "add";
            }
            
            return "user";
        }
        onTriggered: {
            if (typeof accountList.dataModel.data(indexPath).addAccount != "undefined") {
                loginWebViewSheet.loginUrl = sessionWrapper.getLoginUrl();
                loginWebViewSheet.open();
            } else {
                sessionWrapper.changeUserAccount(accountList.dataModel.data(indexPath).accountId);
                accountChanged();
            }
        }
    }
    Container {
        id: activityIndicatorWrapper
        property alias started: activityIndicator.running
        
        layout: DockLayout {

        }
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        
        ActivityIndicator {
            id: activityIndicator
            running: false
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            visible: false
            
            onStarted: {
                accountList.visible = false;
                activityIndicatorWrapper.visible = true;
            }
            onStopped: {
                activityIndicatorWrapper.visible = false;
                accountList.visible = true;
            }
        }
    }
    attachedObjects: [
        LoginWebViewSheet {
            id: loginWebViewSheet
        },
        SessionWrapper {
            id: sessionWrapper
            onUserSessionCreated: {
                loginWebViewSheet.close();
                accountChanged();
            }
        }
    ]
}
