import 'package:flutter/material.dart';
import 'package:untitled/screens/dashboard.dart';
import 'screens/home.dart';
import 'screens/login.dart';
import 'screens/signup.dart';

void main() => runApp(TC());

class TC extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      initialRoute: '/login',
      debugShowCheckedModeBanner: false,
      routes: {
        '/': (context) => Dashboard(),
        '/login': (context) => LoginPage(),
        '/signup' :(context) => SignupPage(),
      },
    );
  }
}