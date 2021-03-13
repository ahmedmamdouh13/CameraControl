class Constants {

    static final String MASTER_BRANCH = 'master'

    static final String QA_BUILD = 'Debug'
    static final String RELEASE_BUILD = 'Release'

    static final String INTERNAL_TRACK = 'internal'
    static final String RELEASE_TRACK = 'internal'
}

def getBuildType() {
    switch (env.BRANCH_NAME) {
        case Constants.MASTER_BRANCH:
            return Constants.RELEASE_BUILD
        default:
            return Constants.QA_BUILD
    }
}

def getTrackType() {
    switch (env.BRANCH_NAME) {
        case Constants.MASTER_BRANCH:
            return Constants.RELEASE_TRACK
        default:
            return Constants.INTERNAL_TRACK
    }
}

def isDeployCandidate() {
    return ("${env.BRANCH_NAME}" =~ /(develop|master)/)
}

pipeline {
    agent { dockerfile true }
    environment {
        appName = 'jenkins-blog'

        KEY_PASSWORD = credentials('keyPass')
        KEY_ALIAS = credentials('alias')
        KEYSTORE = credentials('keystore')
        STORE_PASSWORD = credentials('storePass')
    }

stages {

        stage('Build Bundle') {
            when { expression { return isDeployCandidate() } }
            steps {
                echo 'Building'
                script {
                    VARIANT = getBuildType()
                    sh "./gradlew -PstorePass=${STORE_PASSWORD} -Pkeystore=${KEYSTORE} -Palias=${KEY_ALIAS} -PkeyPass=${KEY_PASSWORD} bundle${VARIANT}"
                }
            }


        }
 stage('Run Tests') {
            steps {
                echo 'Running Tests'
                script {
                    VARIANT = getBuildType()
                    sh "./gradlew test${VARIANT}UnitTest"
                }
                echo 'Running Instrumented Tests'
                script {
                   sh "$ANDROID_HOME/emulator/emulator -avd pixel -no-audio -no-window -no-accel"
                }
                script {

                    sh  "WAIT_CMD=\"$ANDROID_HOME/platform-tools/adb wait-for-device shell getprop init.svc.bootanim\" until $WAIT_CMD | grep -m 1 stopped; do"
                          echo 'Waiting...'
                          }
                   script {
                     sh "sleep 1"
                     }
                   script {
                             sh "$ANDROID_HOME/platform-tools/adb shell input keyevent 82"
                      }
                                      script {
                      sh "$ANDROID_HOME/platform-tools/adb logcat -c"
                      }
                                      script {
                     sh "$ANDROID_HOME/platform-tools/adb logcat > build/logcat.log"
                     }
                script {
                      sh "./gradlew connectedAndroidTest -i"
                    }
            }


                        post {
                               always {
                                       jiraSendBuildInfo site: 'ahmedmamdouh13.atlassian.net'
                                      }
                              }
        }
        stage('Deploy App to Store') {
            when { expression { return isDeployCandidate() } }
            steps {
                echo 'Deploying'
                script {
                    VARIANT = getBuildType()
                    TRACK = getTrackType()

                    if (TRACK == Constants.RELEASE_TRACK) {
                        timeout(time: 5, unit: 'MINUTES') {
                            input "Proceed with deployment to ${TRACK}?"
                        }
                    }



                    androidApkUpload googleCredentialsId: 'CountDownProject',
                            filesPattern: "**/outputs/bundle/${VARIANT.toLowerCase()}/*.aab",
                            trackName: TRACK,
                         rolloutPercentage: '0',
                        releaseName: '2.0.0'

                }
            }
        }
    }
}