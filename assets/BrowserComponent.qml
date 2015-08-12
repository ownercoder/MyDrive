import bb.cascades 1.3
import bb.cascades.pickers 1.0
import com.slytech.filesdatamodel 1.0
import com.slytech.googlefiles 1.0
import com.slytech.filesdatamodel 1.0

Page {
    id: root
    property alias destFolder: filesDataModel.parentId
    property alias folderType: filesDataModel.folderType
    property alias isGrid: driveBrowser.isGrid
    property alias title: titleBar.title
    property alias uploadEnabled: uploadButton.enabled
    property alias createFolderEnabled: createFolderButton.enabled
    
    property string navigationName: ""
    
    titleBar: TitleBar {
        id: titleBar
        branded: TriBool.True
        appearance: TitleBarAppearance.Branded
    
    }
    Container {
        layout: DockLayout { }
        DriveBrowser {
            id: driveBrowser
            dataModel: filesDataModel
            onFolderChoosen: {
                console.log(root.navigationName);
                var child = componentBrowser.createObject();
                child.navigationName = navigationName;
                child.destFolder = id;
                child.isGrid = settings_.directoryView(child.folderType + child.destFolder);
                child.title = folderName;
                child.uploadEnabled = uploadEnabled;
                child.createFolderEnabled = createFolderEnabled;
                eval(navigationName + ".push(child);");
            }
            onBrowserLayoutChanged: {
                if (driveBrowser.isGrid) {
                    listGridButton.title = qsTr("List");
                    listGridButton.imageSource = "asset:///images/ic_view_list.png";
                } else {
                    listGridButton.title = qsTr("Grid");
                    listGridButton.imageSource = "asset:///images/ic_view_grid.png";
                }
            }
        }
        Container {
            id: activityIndicatorWrapper
            property alias started: activityIndicator.running
            
            layout: DockLayout {}
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            
            ActivityIndicator {
                id: activityIndicator
                running: false
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
                visible: false
                
                onStarted: {
                    driveBrowser.visible = false;
                    activityIndicatorWrapper.visible = true;
                }
                onStopped: {
                    activityIndicatorWrapper.visible = false;
                    driveBrowser.visible = true;
                }
            }
        }
    }
    actions: [
        ActionItem {
            title: qsTr("Search")
            ActionBar.placement: ActionBarPlacement.Signature
            imageSource: "asset:///images/bb_action_search.png"
            onTriggered: {
                searchSheet.open();
            }
        },
        ActionItem {
            id: listGridButton
            
            onTriggered: {
                if (driveBrowser.isGrid) {
                    driveBrowser.isGrid = false;
                } else {
                    driveBrowser.isGrid = true;
                }
                settings_.storeDirectoryView(root.folderType + root.destFolder, driveBrowser.isGrid);
            }
            ActionBar.placement: ActionBarPlacement.OnBar
        },
        ActionItem {
            id: uploadButton
            enabled: false
            
            title: qsTr("Upload")
            onTriggered: {
                driveBrowser.uploadFiles();
            }
            
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        ActionItem {
            id: createFolderButton
            enabled: false
            
            title: qsTr("Create folder")
            onTriggered: {
                driveBrowser.createDirectory();
            }
            
            ActionBar.placement: ActionBarPlacement.InOverflow
        }
    ]
    attachedObjects: [
        ComponentDefinition {
            id: componentBrowser
            source: "asset:///BrowserComponent.qml"
        },
        SearchSheet {
            id: searchSheet
            onOpenFolder: {
                driveBrowser.folderChoosen(id, folderName);
                searchSheet.close();
            }
        },
        FilesDataModel {
            id: filesDataModel
            onFinishLoad: {
                activityIndicatorWrapper.started = false;
            }
            onStartLoad: {
                activityIndicatorWrapper.started = true;
            }
        }
    ]
}