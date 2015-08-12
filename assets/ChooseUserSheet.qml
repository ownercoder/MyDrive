import bb.cascades 1.3
import com.slytech.accountdatamodel 1.0
import my.library 1.0

Sheet {
    id: sheetChooseAccount
    
    signal reloadMainWindow();

    Page {
        titleBar: TitleBar {
            title: qsTr("Choose an account")
            dismissAction: ActionItem {
                title: "close"
                onTriggered: {
                    sheetChooseAccount.close();
                }
            }
        }
        AccountList {
            id: accountList
            dataModel: accountDataModel
            onAccountChanged: {
                reloadMainWindow();
                sheetChooseAccount.close();
            }
        }
    }
    onOpened: {
        accountList.activityState = true;
        accountDataModel.load();
    }
    onClosed: {
        accountList.activityState = true;
    }
    attachedObjects: [
        AccountDataModel {
            id: accountDataModel
            onStartLoad: {
                accountList.activityState = true;
            }
            onFinishLoad: {
                accountList.activityState = false;
            }
        }
    ]
}