import bb.cascades 1.3
import my.library 1.0

Container {
    layout: DockLayout { }
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill

    
    WebImageView {
        id: imageView
        url: ListItemData.thumbnailLink
        objectName: "imageView" + ListItemData.id
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        mimeImage: ListItemData.mimeTypeImage
        scalingMethod: ScalingMethod.AspectFill
        filterColor: Color.create(ListItemData.folderColorRgb)
    }
    ImageView {
        id: imageViewFolderShadow
        imageSource: "asset:///images/folder-shadow.png"
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
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
    Container {
        verticalAlignment: VerticalAlignment.Bottom
        bottomPadding: ui.du(1)
        topPadding: ui.du(3)
        leftPadding: ui.du(1)
        rightPadding: ui.du(1)
        layout: DockLayout {
        }
        minHeight: ui.du(10)
        background: ListItemData.isFolder ? Color.Transparent : back.imagePaint
        
        Label {
            id: title
            text: ListItemData.title
            textStyle.fontWeight: FontWeight.W500
            verticalAlignment: VerticalAlignment.Top
            textStyle.fontSize: FontSize.XSmall
            margin.bottomOffset: ui.du(3)
            textStyle.color: Color.White
        }
        Label {
            id: status
            text: Qt.formatDateTime(new Date(ListItemData.modifiedDate), "MMM d, yyyy hh:mm")
            //text: ListItemData.modifiedDate.toString("MMM d, yyyy hh:mm")
            verticalAlignment: VerticalAlignment.Bottom
            textStyle.fontWeight: FontWeight.W400
            textStyle.fontSize: FontSize.XXSmall
            textStyle.color: Color.White
        }
        attachedObjects: [
            ImagePaintDefinition {
                id: back
                repeatPattern: RepeatPattern.X
                imageSource: "asset:///images/header-gradient.png"
            }
        ]
        horizontalAlignment: HorizontalAlignment.Fill
    }
}