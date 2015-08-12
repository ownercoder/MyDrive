import bb.cascades 1.3
import com.slytech.filesdatamodel 1.0
import com.slytech.googlefiles 1.0
import com.slytech.sessionwrapper 1.0

TabbedPane {
    id: tabbedPaneRoot
    Menu.definition: [
        MenuDefinition {
            actions: [
                ActionItem {
                    title: qsTr("User choose")
                    imageSource: "asset:///images/change-user-128.png"
                    onTriggered: {
                        chooseUserSheet.open();
                    }
                },
                ActionItem {
                    title: qsTr("Exit")
                    imageSource: "asset:///images/exit-128.png"
                    onTriggered: {
                        sessionWrapper.exitCurrentSession();
                        chooseUserSheet.open();
                    }
                }
            ]
        }
    ]
    Tab {
        content: NavigationPane {
            id: myFolderNavigationPane
            onCreationCompleted: {
                tabbedPaneRoot.activeTab.loadData();
            }
            onPopTransitionEnded: {
                page.destroy();
            }
        }// END NAVIGATION PANE
        function loadData() {
            if (!myFolderNavigationPane.count()) {
                var child = componentBrowser.createObject(myFolderNavigationPane);
                //child.componentBrowserActions = myFolderActions.actions;
                child.folderType = GoogleFiles.MyFolder;
                child.navigationName = "myFolderNavigationPane";
                child.destFolder = "root";
                child.isGrid = settings_.directoryView(child.folderType + child.destFolder);
                child.title = qsTr("My Drive");
                child.uploadEnabled = true;
                child.createFolderEnabled = true;
                myFolderNavigationPane.push(child);
            }
        }
        title: qsTr("My Drive")
        attachedObjects: [
            ActionSet {
                id: myFolderActions
                actions: [
                    ActionItem {
                        title: qsTr("Some Action")
                    }
                ]
            },
            ActionItem {
                id: uploadButton
                title: qsTr("Upload")
            }
        ]
    }
    Tab {
        content: NavigationPane {
            id: sharedWithNavigationPane
            onPopTransitionEnded: {
                page.destroy();
            }
        }// END NAVIGATION PANE
        function loadData() {
            if (!sharedWithNavigationPane.count()) {
                var child = componentBrowser.createObject(sharedWithNavigationPane);
                child.folderType = GoogleFiles.SharedWithMe;
                child.navigationName = "sharedWithNavigationPane";
                child.destFolder = "root";
                child.isGrid = settings_.directoryView(child.folderType + child.destFolder);
                child.title = qsTr("Shared with me");
                child.uploadEnabled = true;
                child.createFolderEnabled = true;
                sharedWithNavigationPane.push(child);
            }
        }
        title: qsTr("Shared with me")
    }
    Tab {
        content: NavigationPane {
            id: starredWithNavigationPane
            onPopTransitionEnded: {
                page.destroy();
            }
        }// END NAVIGATION PANE
        function loadData() {
            if (!starredWithNavigationPane.count()) {
                var child = componentBrowser.createObject(starredWithNavigationPane);
                child.folderType = GoogleFiles.Starred;
                child.navigationName = "starredNavigationPane";
                child.destFolder = "root";
                child.isGrid = settings_.directoryView(child.folderType + child.destFolder);
                child.title = qsTr("Starred");
                child.uploadEnabled = true;
                child.createFolderEnabled = true;
                starredWithNavigationPane.push(child);
            }
        }
        title: qsTr("Starred")
    }
    Tab {
        content: NavigationPane {
            id: trashedNavigationPane
            onPopTransitionEnded: {
                page.destroy();
            }
        }// END NAVIGATION PANE
        function loadData() {
            if (!trashedNavigationPane.count()) {
                var child = componentBrowser.createObject(trashedNavigationPane);
                child.folderType = GoogleFiles.Trashed;
                child.navigationName = "trashedNavigationPane";
                child.destFolder = "root";
                child.isGrid = settings_.directoryView(child.folderType + child.destFolder);
                child.title = qsTr("Trashed");
                trashedNavigationPane.push(child);
            }
        }
        title: qsTr("Trashed")
    }
    onActiveTabChanged: {
        console.log(activeTab.title);
        activeTab.loadData();
    }
    attachedObjects: [
        ComponentDefinition {
            id: componentBrowser
            source: "asset:///BrowserComponent.qml"
        },
        ChooseUserSheet {
            id: chooseUserSheet
            onReloadMainWindow: {
                for(var i=0;i < myFolderNavigationPane.count();i++) {
                    myFolderNavigationPane.pop().destroy();
                }
                for(var i=0;i < sharedWithNavigationPane.count();i++) {
                    sharedWithNavigationPane.pop().destroy();
                }
                for(var i=0;i < starredWithNavigationPane.count();i++) {
                    starredWithNavigationPane.pop().destroy();
                }
                for(var i=0;i < starredWithNavigationPane.count();i++) {
                    starredWithNavigationPane.pop().destroy();
                }
                for(var i=0;i < trashedNavigationPane.count();i++) {
                    trashedNavigationPane.pop().destroy();
                }
                tabbedPaneRoot.activeTab.loadData();
                myFolderNavigationPane.creationCompleted();
            }
        },
        SessionWrapper {
            id: sessionWrapper
        }
    ]
}