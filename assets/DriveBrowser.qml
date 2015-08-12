import bb.cascades 1.3
import bb.system 1.2
import bb.device 1.3
import bb.cascades.pickers 1.0
import com.slytech.googlefiles 1.0
import com.slytech.googledownloader 1.0
import com.slytech.googlefileinvoker 1.0
import com.slytech.apiwrapper 1.0
import com.slytech.googleuploader 1.0
import my.library 1.0
import "library.js" as JsHelper

Container {
    objectName: "driveBrowser"
    layout: DockLayout {
    }
    id: idDriveBrowser
    signal folderChoosen(string id, string folderName)
    signal browserLayoutChanged()
    signal needToFetchMore()

    property alias listLayout: listView.layout
    property bool isGrid: false
    property alias dataModel: listView.dataModel
    
    onIsGridChanged: {
        if (isGrid) {
            listView.layout = gridLayout;
        } else {
            listView.layout = stackLayout;
        }
        browserLayoutChanged();
    }

    ListView {
        id: listView
        listItemComponents: [
            ListItemComponent {
                type: "gridItem"
                CustomListItem {
                    content: FilesGridLayout {
                    }
                    highlightAppearance: HighlightAppearance.Frame
                    contextActions: GoogleActionSet {
                    }
                }

            },
            ListItemComponent {
                type: "listItem"
                CustomListItem {
                    content: FilesListLayout {
                    }
                    contextActions: GoogleActionSet {
                    }
                    highlightAppearance: HighlightAppearance.Frame
                }
            },
            ListItemComponent {
                type: "fetchMoreItem"
                CustomListItem {
                    FetchMoreItemLayout {
                        
                    }
                }
            },
            ListItemComponent {
                type: "loadingItem"
                CustomListItem {
                    LoadingItemLayout {
                        
                    }
                }
            }
        ]
        multiSelectHandler {
            actions: [
                DeleteActionItem {
                    title: qsTr("Delete")
                },
                ActionItem {
                    title: qsTr("Some action")
                }
            ]
        }
        flickMode: FlickMode.SingleItem
        
        function itemType(data, indexPath) {
            if (typeof listView.dataModel.data(indexPath).fetchMoreItem != "undefined" && listView.dataModel.data(indexPath).fetchMoreItem) {
                return "fetchMoreItem";
            }
            if (typeof listView.dataModel.data(indexPath).loadingItem != "undefined" && listView.dataModel.data(indexPath).loadingItem) {
                return "loadingItem";
            }
            if (isGrid) {
                return "gridItem"
            }
            return "listItem"
        }
        onSelectionChanged: {
            console.log(selectionList().length);
            if (selectionList().length > 1) {
                multiSelectHandler.status = qsTr("%1 items selected").arg(selectionList().length);
            } else if (selectionList().length == 1) {
                multiSelectHandler.status = qsTr("1 item selected");
            } else {
                multiSelectHandler.status = qsTr("None selected");
            }
        }
        onTriggered: {
            if (listView.multiSelectHandler.active) {
                return;
            }
            if (listView.dataModel.data(indexPath).isFolder) {
                folderChoosen(listView.dataModel.data(indexPath).id, listView.dataModel.data(indexPath).title);
            } else if (listView.dataModel.data(indexPath).fetchMoreItem != "undefined" && listView.dataModel.data(indexPath).fetchMoreItem) {
                needToFetchMore();
            } else {
                var fileData = listView.dataModel.data(indexPath);
                googleDownloader.setProperty("previewAfterFinish", true);
                googleDownloader.addToQueue(fileData)
                googleDownloader.downloadToTemp();
                if (!JsHelper.objectSize(fileData.exportLinks)) {
                    progressDownloadingDialog.body = qsTr("Downloading file: %1").arg(fileData.title);
                    progressDownloadingDialog.progress = 0;
                    progressDownloadingDialog.update();
                    progressDownloadingDialog.show();
                }
            }
        }
        onLayoutChanged: {
            if (isGrid) {
                listView.leftPadding = ui.du(1)
                listView.rightPadding = ui.du(1)
            } else {
                listView.leftPadding = 0;
                listView.rightPadding = 0;
            }
            listView.dataModel.itemsChanged(DataModelChangeType.Init);
        }
        onCreationCompleted: {
            browserLayoutChanged();
        }
    }
    
    function uploadFiles() {
        filesForUpload.open();
    }
    function createDirectory() {
        folderNamePromt.show();
    }
    function trashFile(itemData) {
        apiWrapperDialog.body = qsTr("Removing file: %1").arg(itemData.title);
        apiWrapper.addToQueue(itemData);
        apiWrapper.trash();
    }
    function downloadTo(destination, itemdata) {
        googleDownloader.recursion = true;
        googleDownloader.setProperty("previewAfterFinish", false);
        googleDownloader.addToQueue(itemdata);
        googleDownloader.downloadTo(destination);
        
        progressDownloadingDialog.body = qsTr("Downloading: %1").arg(itemdata.title);
        progressDownloadingDialog.progress = 0;
        progressDownloadingDialog.show();
    }
    
    attachedObjects: [
        DisplayInfo {
            id: displayInfoAdd
        },
        StackListLayout {
            id: stackLayout
        },
        GridListLayout {
            id: gridLayout
            columnCount: displayInfoAdd.physicalSize.width < 60 ? 3 : 4
            horizontalCellSpacing: ui.du(1)
            verticalCellSpacing: ui.du(1)
            cellAspectRatio: 1
        },
        GoogleDriveDownloader {
            id: googleDownloader
            onDownloadStarted: {
                progressDownloadingDialog.body = qsTr("Downloading file: %1").arg(filename);
                //progressDownloadingDialog.progress = 0;
                progressDownloadingDialog.update();
            }
            onDownloadCompleted: {
                progressDownloadingDialog.cancel();
                if (googleDownloader.property("previewAfterFinish")) {
                    fileInvoker.filename = localfile;
                    fileInvoker.open(mimeType);
                    console.log(localfile + " mime: " + mimeType);
                }
            }
            onBatchDownloadCompleted: {
                progressDownloadingDialog.cancel();
            }
            onError: {
                progressDownloadingDialog.cancel();
                console.log(errorText);
                messageToast.body = errorText;
                messageToast.show();
            }
            onDownloadProgress: {
                if (bytesReceived == 0 || bytesTotal == 0) {
                    var percent = 0;
                } else {
                    var percent = parseInt(bytesReceived / (bytesTotal / 100));
                }
                progressDownloadingDialog.progress = percent;
                progressDownloadingDialog.statusMessage = percent + "% ...";
                //console.log("Downloaded: " + percent + "%");
                progressDownloadingDialog.update();
            }
            onChooseExportFileFormart: {
                exportFormatDialog.title = qsTr("Choose export format");
                exportFormatDialog.cancelButton.label = qsTr("cancel");
                exportFormatDialog.confirmButton.label = qsTr("export");
                exportFormatDialog.clearList();
                var humanFormats = JsHelper.mimeTypesToNames(formats);
                exportFormatDialog.appendItems(humanFormats);
                exportFormatDialog.show();
            }
        },
        GoogleFileInvoker {
            id: fileInvoker
            onError: {
                console.log(error);
                messageToast.body = error;
                messageToast.show();
            }
        },
        SystemProgressDialog {
            id: progressDownloadingDialog
            progress: -1
            title: qsTr("File downloading ...")
            confirmButton.enabled: false
            cancelButton.enabled: true
            cancelButton.label: qsTr("cancel")
            onFinished: {
                googleDownloader.abort();
            }
        },
        SystemProgressDialog {
            id: progressUploadingDialog
            progress: -1
            title: qsTr("File uploading ...")
            confirmButton.enabled: false
            cancelButton.enabled: true
            cancelButton.label: qsTr("cancel")
            onFinished: {
                driveUploader.abort();
            }
            
        },
        SystemProgressDialog {
            id: apiWrapperDialog
            progress: -1
            confirmButton.enabled: false
            cancelButton.enabled: true
            onFinished: {
                apiWrapper.abort();
            }
        },
        SystemToast {
            id: messageToast
        },
        SystemListDialog {
            id: exportFormatDialog
            title: qsTr("Choose export format")
            body: qsTr("Format does not supported, please choose convert variant below:")
            confirmButton.label: qsTr("Convert")
            cancelButton.label: qsTr("Cancel")
            onFinished: {
                if (value == SystemUiResult.ConfirmButtonSelection) {
                    progressDownloadingDialog.body = qsTr("Document exporting");
                    progressDownloadingDialog.progress = 0;
                    progressDownloadingDialog.update();
                    progressDownloadingDialog.show();
                    googleDownloader.continueDownload(exportFormatDialog.selectedIndices[0], rememberMeChecked);
                }
            }
            includeRememberMe: true
            rememberMeText: qsTr("Remember my choose")
            rememberMeChecked: true
        },
        SystemPrompt {
            id: folderNamePromt
            title: qsTr("Add folder")
            emoticonsEnabled: false
            inputField.defaultText: qsTr("New folder")
            inputField.onDefaultTextChanged: {
                if (value == "") {
                    folderNamePromt.confirmButton.enabled = false;
                } else {
                    folderNamePromt.confirmButton.enabled = true;
                }
            }
            onFinished: {
                if (value == SystemUiResult.ConfirmButtonSelection) {
                    apiWrapperDialog.title = qsTr("Add folder");
                    apiWrapperDialog.body = qsTr("Creating folder: %1").arg(inputFieldTextEntry());
                    apiWrapper.createDirectory(inputFieldTextEntry(), listView.dataModel.parentId);
                }
            }
        },
        ApiWrapper {
            id: apiWrapper
            onError: {
                messageToast.body = text;
                messageToast.show();
            }
            onRequestStarted: {
                apiWrapperDialog.update();
                apiWrapperDialog.show();
            }
            onRequestFinished: {
                listView.dataModel.load();
                apiWrapperDialog.cancel();
                apiWrapperDialog.resetBody();
            }
        },
        GoogleDriveUploader {
            id: driveUploader
            onError: {
                console.log(errorText);
            }
            onUploadCompleted: {
                dataModel.load();
                progressUploadingDialog.cancel();
                progressUploadingDialog.body = "";
                progressUploadingDialog.progress = 0;
            }
            onUploadProgress: {
                progressUploadingDialog.body = qsTr("Uploading file: %1").arg(fileName);
                var percent = parseInt(bytesSent / (bytesTotal / 100));
                progressUploadingDialog.progress = percent;
                progressUploadingDialog.statusMessage = percent + "% ...";
                progressUploadingDialog.update();
            }
            onUploadStarted: {
                progressUploadingDialog.show();
            }
        },
        FilePicker {
            id: filesForUpload
            mode: FilePickerMode.PickerMultiple
            type: FileType.Other
            title: qsTr("Choose files")
            onFileSelected: {
                for(var i = 0; i < selectedFiles.length;i++) {
                    driveUploader.addToQueue(selectedFiles[i]);
                }
                
                console.log("Upload to: " + dataModel.parentId);
                driveUploader.uploadTo(dataModel.parentId);
            }
            sourceRestriction: FilePickerSourceRestriction.LocalOnly
        }
    ]
}
