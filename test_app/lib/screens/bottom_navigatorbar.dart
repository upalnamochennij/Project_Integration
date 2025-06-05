import 'package:flutter/material.dart';
import '../screens/homescreen.dart';
import '../functions/weight_bmi_screen.dart';

class VitalTrackHome extends StatefulWidget {
  const VitalTrackHome({super.key});

  @override
  State<VitalTrackHome> createState() => _VitalTrackHomeState();
}

class _VitalTrackHomeState extends State<VitalTrackHome> {
  int _selectedIndex = 0;

  final List<Widget> _screens = const [
    VitalMainScreen(),
    WeightBMIScreen(),
  ];

  void _onItemTapped(int index) {
    setState(() {
      _selectedIndex = index;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: _screens[_selectedIndex],
      bottomNavigationBar: BottomNavigationBar(
        currentIndex: _selectedIndex,
        onTap: _onItemTapped,
        selectedItemColor: Colors.redAccent,
        items: const [
          BottomNavigationBarItem(
            icon: Icon(Icons.monitor_heart),
            label: 'Vitals',
          ),
          BottomNavigationBarItem(
            icon: Icon(Icons.fitness_center),
            label: 'BMI Tracker',
          ),
        ],
      ),
    );
  }
}

