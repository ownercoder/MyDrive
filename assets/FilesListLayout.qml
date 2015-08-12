import bb.cascades 1.3
import my.library 1.0
import "library.js" as JsHelper

Container {
    preferredHeight: ui.du(12)
    layout: DockLayout {
    }
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill
    topPadding: ui.du(1)
    bottomPadding: ui.du(1)
    leftPadding: ui.du(1)
    rightPadding: ui.du(1)
    Container {
        preferredWidth: ui.du(12)
        preferredHeight: ui.du(12)
        layout: DockLayout {
        
        }
        WebImageView {
            id: imageView
            url: ListItemData.thumbnailLink
            objectName: "imageView" + ListItemData.id
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top
            mimeImage: ListItemData.mimeTypeImage
            scalingMethod: ScalingMethod.AspectFill
            filterColor: ListItemData.folderColorRgb ? Color.create(ListItemData.folderColorRgb) : null
        }
        ImageView {
            id: imageViewFolderShadow
            imageSource: "asset:///images/folder-shadow.png"
            scalingMethod: ScalingMethod.AspectFill
            visible: ListItemData.isFolder
        }
        ImageView {
            id: imageViewFolderStar
            imageSource: "asset:///images/overlay-star.png"
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            scalingMethod: ScalingMethod.AspectFill
            visible: ListItemData.labels.starred
        }
    }
    Container {
        layout: DockLayout {
        }
        leftPadding: ui.du(13)
        topPadding: ui.du(1)
        bottomPadding: ui.du(1)
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        Label {
            text: ListItemData.title
            verticalAlignment: VerticalAlignment.Top
            textStyle.fontWeight: FontWeight.W500
            textStyle.fontSize: FontSize.Small
        }
        Label {
            id: status
            text: Qt.formatDateTime(new Date(ListItemData.modifiedDate), "MMM d, yyyy hh:mm")
            //text: ListItemData.modifiedDate.toString("MMM d, yyyy hh:mm")
            verticalAlignment: VerticalAlignment.Bottom
            textStyle.fontWeight: FontWeight.W400
            textStyle.fontSize: FontSize.XSmall
        }
        Label {
            horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Center
            text: JsHelper.humanFileSize(ListItemData.fileSize)
            textStyle.fontWeight: FontWeight.W400
            textStyle.fontSize: FontSize.XXSmall
            textStyle.color: Color.create("#ff8e8e8e")
        }
    }
}