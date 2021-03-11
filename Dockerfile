FROM adoptopenjdk/openjdk8:alpine

ENV SDK_TOOLS "4333796"
ENV ANDROID_HOME "/opt/sdk"
ENV ANDROID_NDK_HOME "/opt/sdk/ndk-bundle"
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

WORKDIR /home/android
RUN chmod -R 777 "$ANDROID_HOME"
RUN chmod -R 777 "$ANDROID_NDK_HOME"
# Platforms we need (defaults)
ARG SDK_PACKAGES="build-tools;30.0.2 platforms;android-30"
RUN sdkmanager $SDK_PACKAGES

RUN sdkmanager --install "ndk;20.0.5594570" --channel=0 // Install the NDK from the canary channel (or below)
RUN sdkmanager --install "cmake;10.24988404" // Install a specific version of CMake


# User for our build, depends on your system
RUN adduser -u 1000 -h /home/android -D jenkins
USER jenkins


# Common Gradle settings, customise as you see fit
ENV GRADLE_OPTS "-Xmx1600m -Dorg.gradle.daemon=false -Dorg.gradle.parallel=true -Dorg.gradle.caching=true"
# Move the Gradle home / cache into the workspace dir
ENV GRADLE_USER_HOME ./gradle-cache