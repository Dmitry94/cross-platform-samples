import QtQuick 2.0

Rectangle {
    id: root
    width: 300; height: 300
    color: "#646464"

    Item {
        id : clock
        width: 300
        height: 300

        property int hours
        property int minutes
        property int seconds
        signal qmlSignal()

        Timer {
            interval: 200; running: true; repeat: true;
            onTriggered: clock.qmlSignal()
        }

        Item {
            anchors.centerIn: parent
            width: 200; height: 240

            Image { id: background; source: "content/clock-night.png"; }

            Image {
                x: 92.5; y: 27
                source: "content/hour.png"
                transform: Rotation {
                    id: hourRotation
                    origin.x: 7.5; origin.y: 73;
                    angle: (clock.hours * 30) + (clock.minutes * 0.5)
                    Behavior on angle {
                        SpringAnimation { spring: 2; damping: 0.2; modulus: 360 }
                    }
                }
            }

            Image {
                x: 93.5; y: 17
                source: "content/minute.png"
                transform: Rotation {
                    id: minuteRotation
                    origin.x: 6.5; origin.y: 83;
                    angle: clock.minutes * 6
                    Behavior on angle {
                        SpringAnimation { spring: 2; damping: 0.2; modulus: 360 }
                    }
                }
            }

            Image {
                x: 97.5; y: 20
                source: "content/second.png"
                transform: Rotation {
                    id: secondRotation
                    origin.x: 2.5; origin.y: 80;
                    angle: clock.seconds * 6
                    Behavior on angle {
                        SpringAnimation { spring: 2; damping: 0.2; modulus: 360 }
                    }
                }
            }

            Image {
                anchors.centerIn: background; source: "content/center.png"
            }
        }
    }
}
