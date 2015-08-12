import bb.cascades 1.3
import com.slytech.searchdatamodel 1.0
import my.library 1.0

Sheet {
    id: searchSheet
    signal openFolder(string id, string folderName)
    NavigationPane {
        id: searchNavigationPane
        Page {
            titleBar: TitleBar {
                kind: TitleBarKind.TextField
                kindProperties: TextFieldTitleBarKindProperties {
                    id: titleBarKindProerties
                    textField.hintText: qsTr("Search file")
                    textField.onTextChanging: {
                        searchDataModel.searchFile(text);
                    }
                }
                dismissAction: ActionItem {
                    title: qsTr("cancel")
                    onTriggered: {
                        searchSheet.close();
                    }
                }
            }
            
            DriveBrowser {
                dataModel: searchDataModel
                onNeedToFetchMore: {
                    searchDataModel.fetchMore();
                }
                onFolderChoosen: {
                    openFolder(id, folderName);
                }
            }
        }
    }
    attachedObjects: [
        SearchDataModel {
            id: searchDataModel
        }
    ]
    onClosed: {
        titleBarKindProerties.textField.text = "";
    }
}
