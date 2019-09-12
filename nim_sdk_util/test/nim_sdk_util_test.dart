import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:nim_sdk_util/nim_sdk_util.dart';

void main() {
  const MethodChannel channel = MethodChannel('nim_sdk_util');

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

  test('getPlatformVersion', () async {
    expect(await NimSdkUtil.platformVersion, '42');
  });
}
