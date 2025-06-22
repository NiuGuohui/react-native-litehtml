module.exports = {
  dependency: {
    platforms: {
      /**
       * @type {import('@react-native-community/cli-types').IOSDependencyParams}
       */
      ios: {},
      /**
       * @type {import('@react-native-community/cli-types').AndroidDependencyParams}
       */
      android: {
        cxxModuleCMakeListsModuleName: 'react-native-litehtml',
        cxxModuleCMakeListsPath: 'CMakeLists.txt',
        cxxModuleHeaderName: 'NativeLitehtmlModule',
      },
    },
  },
};
