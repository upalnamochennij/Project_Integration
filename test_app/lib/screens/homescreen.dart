import 'package:flutter/material.dart';
import '../functions/vital_trend.dart';
import 'measurement_chart_screen.dart';
import '../widgets/vital_card.dart';
import 'settings_screen.dart';

class VitalMainScreen extends StatelessWidget {
  const VitalMainScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color(0xFFFFF4F2),
      body: Column(
        children: [
          Container(
            padding: const EdgeInsets.symmetric(vertical: 20, horizontal: 16),
            color: const Color(0xFFFFDADC),
            child: SafeArea(
              bottom: false,
              child: Stack(
                alignment: Alignment.center,
                children: [
                  Align(
                    alignment: Alignment.centerLeft,
                    child: IconButton(
                      icon: const Icon(Icons.settings, color: Colors.black87),
                      onPressed: () {
                        Navigator.push(
                          context,
                          MaterialPageRoute(builder: (_) => const SettingsScreen()),
                        );
                      },
                    ),
                  ),
                  const Center(
                    child: Text(
                      'VITALWATCH',
                      style: TextStyle(
                        fontSize: 20,
                        fontWeight: FontWeight.bold,
                        letterSpacing: 1.5,
                        color: Colors.black87,
                      ),
                    ),
                  ),
                ],
              ),
            ),
          ),

          Expanded(
            child: SafeArea(
              top: false,
              child: Padding(
                padding: const EdgeInsets.symmetric(horizontal: 24, vertical: 20),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    const Text(
                      'Welcome to\nVitalWatch 🩺',
                      style: TextStyle(fontSize: 30, fontWeight: FontWeight.w700, height: 1.2),
                    ),
                    const SizedBox(height: 12),
                    const Text(
                      'Your latest measurements',
                      style: TextStyle(fontSize: 16, color: Colors.black54),
                    ),
                    const SizedBox(height: 24),
                    VitalCard(
                      icon: Icons.favorite,
                      label: 'Heart Rate',
                      value: '72 bpm',
                      color: Colors.red,
                      bgColor: const Color(0xFFFFE5E7),
                      onTap: () {
                        Navigator.push(
                          context,
                          MaterialPageRoute(
                            builder: (_) => const MeasurementChartScreen(
                              title: 'Heart Rate',
                              value: '72 bpm',
                              data: [65, 70, 68, 75, 69, 77, 72, 80],
                              lineColor: Colors.red,
                            ),
                          ),
                        );
                      },
                    ),
                    VitalCard(
                      icon: Icons.directions_walk,
                      label: 'Steps',
                      value: '200 steps',
                      color: Colors.blue,
                      bgColor: const Color(0xFFD6ECF8),
                      onTap: () {
                        Navigator.push(
                          context,
                          MaterialPageRoute(
                            builder: (_) => const MeasurementChartScreen(
                              title: 'Steps',
                              value: '200 steps',
                              data: [200, 500, 620, 580, 610, 670, 690],
                              lineColor: Colors.blue,
                            ),
                          ),
                        );
                      },
                    ),
                    VitalCard(
                      icon: Icons.thermostat,
                      label: 'Temperature',
                      value: '24°C',
                      color: Colors.deepOrange,
                      bgColor: const Color(0xFFFFECD4),
                      onTap: () {
                        Navigator.push(
                          context,
                          MaterialPageRoute(
                            builder: (_) => const MeasurementChartScreen(
                              title: 'Temperature',
                              value: '35.5°F',
                              data: [20, 21, 23, 23.5, 24, 25],
                              lineColor: Colors.orange,
                            ),
                          ),
                        );
                      },
                    ),
                    VitalCard(
                      icon: Icons.opacity,
                      label: 'Blood Oxygen',
                      value: '98%',
                      color: Colors.teal,
                      bgColor: const Color(0xFFE4F6EE),
                      onTap: () {
                        Navigator.push(
                          context,
                          MaterialPageRoute(
                            builder: (_) => const MeasurementChartScreen(
                              title: 'Blood Oxygen',
                              value: '98%',
                              data: [96, 97, 98, 97, 99, 98],
                              lineColor: Colors.teal,
                            ),
                          ),
                        );
                      },
                    ),
                    const Spacer(),
                    Center(
                      child: TextButton.icon(
                        style: TextButton.styleFrom(
                          backgroundColor: const Color(0xFFFFE5E7),
                          padding: const EdgeInsets.symmetric(horizontal: 24, vertical: 12),
                          shape: RoundedRectangleBorder(
                            borderRadius: BorderRadius.circular(16),
                          ),
                        ),
                        onPressed: () {
                          Navigator.push(
                            context,
                            MaterialPageRoute(
                              builder: (_) => const VitalTrend(),
                            ),
                          );
                        },
                        label: const Text(
                          'History',
                          style: TextStyle(color: Colors.red, fontSize: 20),
                        ),
                      ),
                    ),
                    const SizedBox(height: 20),
                  ],
                ),
              ),
            ),
          ),
        ],
      ),
    );
  }
}
