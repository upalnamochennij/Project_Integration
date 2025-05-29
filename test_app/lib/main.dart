import 'package:flutter/material.dart';
import 'login_screen/login_page.dart'; // <- add this import

void main() {
  runApp(const VitalTrackApp());
}

class VitalTrackApp extends StatelessWidget {
  const VitalTrackApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'VitalWatch',
      theme: ThemeData(
        primarySwatch: Colors.pink,
        fontFamily: 'Sans',
        scaffoldBackgroundColor: const Color(0xFFFFF1F2),
      ),
      home: const LoginPage(), // <- point to login page
      debugShowCheckedModeBanner: false,
    );
  }
}
