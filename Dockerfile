FROM adoptopenjdk/openjdk8:alpine

ENV SDK_TOOLS "4333796"
ENV ANDROID_HOME "/opt/sdk"
ENV ANDROID_NDK_HOME "/opt/android-ndk"
ENV ANDROID_NDK_VERSION r21e
ENV PATH=$PATH:$ANDROID_HOME/tools:$ANDROID_HOME/tools/bin:$ANDROID_HOME/platform-tools

# Install required dependencies
RUN apk add --no-cache bash git unzip wget && \
    apk add --virtual .rundeps $runDeps && \
    rm -rf /tmp/* && \
    rm -rf /var/cache/apk/*

# Download and extract Android Tools
RUN wget -q https://dl.google.com/android/repository/sdk-tools-linux-${SDK_TOOLS}.zip -O /tmp/tools.zip && \
    mkdir -p ${ANDROID_HOME} && \
    unzip -qq /tmp/tools.zip -d ${ANDROID_HOME} && \
    rm -v /tmp/tools.zip

# Install SDK Packages
RUN mkdir -p ~/.android/ && touch ~/.android/repositories.cfg && \
    yes | ${ANDROID_HOME}/tools/bin/sdkmanager "--licenses" && \
    ${ANDROID_HOME}/tools/bin/sdkmanager "platform-tools" && \
    ${ANDROID_HOME}/tools/bin/sdkmanager "--update"


# download
RUN mkdir /opt/android-ndk-tmp && \
    cd /opt/android-ndk-tmp && \
    wget -q https://dl.google.com/android/repository/android-ndk-${ANDROID_NDK_VERSION}-linux-x86_64.zip && \
# uncompress
    unzip -q android-ndk-${ANDROID_NDK_VERSION}-linux-x86_64.zip && \
# move to its final location
    mv ./android-ndk-${ANDROID_NDK_VERSION} ${ANDROID_NDK_HOME} && \
# remove temp dir
    cd ${ANDROID_NDK_HOME} && \
    rm -rf /opt/android-ndk-tmp


WORKDIR /home/android
RUN chmod -R 777 "$ANDROID_HOME"
RUN chmod -R 777 "$ANDROID_NDK_HOME"
# Platforms we need (defaults)
ARG SDK_PACKAGES="build-tools;30.0.2 platforms;android-30"
RUN sdkmanager $SDK_PACKAGES
RUN sdkmanager --install "ndk;21.4.7075529"  && \
    yes | sdkmanager --licenses


RUN sdkmanager emulator  && \
    yes | sdkmanager --licenses

RUN sdkmanager "system-images;android-30;google_apis;x86_64" && \
    yes | sdkmanager --licenses


ENV ANDROID_AVD_HOME="/opt/sdk/avd"
RUN  mkdir -p $ANDROID_AVD_HOME
RUN chmod -R 777 "$ANDROID_AVD_HOME"


RUN echo "no" | ${ANDROID_HOME}/tools/bin/avdmanager create avd -n pixel -k "system-images;android-30;google_apis;x86_64" -f -p $ANDROID_AVD_HOME


# Add instructions for android emulator
RUN apk add qemu-system-x86_64 libvirt libvirt-daemon dbus polkit qemu-img

RUN chmod -R 777 "/var"
RUN chmod  777 "/"


# User for our build, depends on your system
RUN adduser -u 1000 -h /home/android -D jenkins
RUN adduser jenkins kvm
RUN adduser jenkins qemu
USER jenkins






# Common Gradle settings, customise as you see fit
ENV GRADLE_OPTS "-Xmx1600m -Dorg.gradle.daemon=false -Dorg.gradle.parallel=true -Dorg.gradle.caching=true"
# Move the Gradle home / cache into the workspace dir
ENV GRADLE_USER_HOME ./gradle-cache