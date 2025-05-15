// import 'package:flutter/material.dart';
// import 'package:fl_chart/fl_chart.dart';
//
// void main() {
//   runApp(const VitalTrackApp());
// }
//
// class VitalTrackApp extends StatelessWidget {
//   const VitalTrackApp({super.key});
//
//   @override
//   Widget build(BuildContext context) {
//     return MaterialApp(
//       title: 'VitalTrack',
//       theme: ThemeData(
//         primarySwatch: Colors.pink,
//         fontFamily: 'Sans',
//         scaffoldBackgroundColor: const Color(0xFFFFF1F2),
//       ),
//       home: const VitalTrackHome(),
//       debugShowCheckedModeBanner: false,
//     );
//   }
// }
//
// class VitalTrackHome extends StatelessWidget {
//   const VitalTrackHome({super.key});
//
//   Widget buildVitalCard({
//     required IconData icon,
//     required String label,
//     required String value,
//     required Color color,
//     required Color bgColor,
//     required VoidCallback onTap,
//   }) {
//     return GestureDetector(
//       onTap: onTap,
//       child: Container(
//         margin: const EdgeInsets.symmetric(vertical: 8),
//         padding: const EdgeInsets.symmetric(horizontal: 16, vertical: 12),
//         decoration: BoxDecoration(
//           color: bgColor,
//           borderRadius: BorderRadius.circular(16),
//         ),
//         child: Row(
//           children: [
//             Icon(icon, color: color, size: 28),
//             const SizedBox(width: 16),
//             Text(
//               label,
//               style: const TextStyle(fontSize: 18, fontWeight: FontWeight.w600),
//             ),
//             const Spacer(),
//             Text(
//               value,
//               style: const TextStyle(fontSize: 18, fontWeight: FontWeight.w500),
//             ),
//           ],
//         ),
//       ),
//     );
//   }
//
//   @override
//   Widget build(BuildContext context) {
//     return Scaffold(
//       body: SafeArea(
//         child: Padding(
//           padding: const EdgeInsets.symmetric(horizontal: 24, vertical: 20),
//           child: Column(
//             crossAxisAlignment: CrossAxisAlignment.start,
//             children: [
//               const Center(
//                 child: Text(
//                   'VITALTRACK',
//                   style: TextStyle(
//                     fontSize: 20,
//                     fontWeight: FontWeight.bold,
//                     letterSpacing: 1.5,
//                     color: Colors.black87,
//                   ),
//                 ),
//               ),
//               const SizedBox(height: 24),
//               const Text(
//                 'Welcome to\nVitalTrack 🩺',
//                 style: TextStyle(
//                   fontSize: 28,
//                   fontWeight: FontWeight.w700,
//                   height: 1.2,
//                 ),
//               ),
//               const SizedBox(height: 12),
//               const Text(
//                 'Your latest measurements',
//                 style: TextStyle(fontSize: 16, color: Colors.black54),
//               ),
//               const SizedBox(height: 24),
//               buildVitalCard(
//                 icon: Icons.favorite,
//                 label: 'Heart Rate',
//                 value: '72 bpm',
//                 color: Colors.red,
//                 bgColor: const Color(0xFFFFE5E7),
//                 onTap: () {
//                   Navigator.push(
//                     context,
//                     MaterialPageRoute(
//                       builder: (_) => MeasurementChartScreen(
//                         title: 'Heart Rate',
//                         value: '72 bpm',
//                         data: [65, 70, 68, 75, 69, 77, 72, 80],
//                         lineColor: Colors.red,
//                       ),
//                     ),
//                   );
//                 },
//               ),
//               buildVitalCard(
//                 icon: Icons.assignment,
//                 label: 'Blood Pressure',
//                 value: '120/80 mmHg',
//                 color: Colors.blue[700]!,
//                 bgColor: const Color(0xFFD6ECF8),
//                 onTap: () {
//                   Navigator.push(
//                     context,
//                     MaterialPageRoute(
//                       builder: (_) => MeasurementChartScreen(
//                         title: 'Blood Pressure',
//                         value: '120/80 mmHg',
//                         data: [115, 120, 118, 122, 121, 117, 119],
//                         lineColor: Colors.blue,
//                       ),
//                     ),
//                   );
//                 },
//               ),
//               buildVitalCard(
//                 icon: Icons.thermostat,
//                 label: 'Temperature',
//                 value: '98.6°F',
//                 color: Colors.deepOrange,
//                 bgColor: const Color(0xFFFFF1CD),
//                 onTap: () {
//                   Navigator.push(
//                     context,
//                     MaterialPageRoute(
//                       builder: (_) => MeasurementChartScreen(
//                         title: 'Temperature',
//                         value: '98.6°F',
//                         data: [98.0, 98.3, 98.6, 98.7, 98.4, 98.8],
//                         lineColor: Colors.orange,
//                       ),
//                     ),
//                   );
//                 },
//               ),
//               buildVitalCard(
//                 icon: Icons.opacity,
  //                 label: 'Blood Oxygen',
//                 value: '98%',
//                 color: Colors.teal,
//                 bgColor: const Color(0xFFD9F4F1),
//                 onTap: () {
//                   Navigator.push(
//                     context,
//                     MaterialPageRoute(
//                       builder: (_) => MeasurementChartScreen(
//                         title: 'Blood Oxygen',
//                         value: '98%',
//                         data: [96, 97, 98, 97, 99, 98],
//                         lineColor: Colors.teal,
//                       ),
//                     ),
//                   );
//                 },
//               ),
//               const Spacer(),
//               Center(
//                 child: TextButton.icon(
//                   style: TextButton.styleFrom(
//                     backgroundColor: const Color(0xFFFFE5E7),
//                     padding:
//                     const EdgeInsets.symmetric(horizontal: 24, vertical: 12),
//                     shape: RoundedRectangleBorder(
//                       borderRadius: BorderRadius.circular(16),
//                     ),
//                   ),
//                   onPressed: () {},
//                   icon: const Icon(Icons.add, color: Colors.red),
//                   label: const Text(
//                     'Add Measurement',
//                     style: TextStyle(color: Colors.red, fontSize: 16),
//                   ),
//                 ),
//               ),
//               const SizedBox(height: 20),
//             ],
//           ),
//         ),
//       ),
//     );
//   }
// }
//
// class MeasurementChartScreen extends StatelessWidget {
//   final String title;
//   final String value;
//   final List<double> data;
//   final Color lineColor;
//
//   const MeasurementChartScreen({
//     super.key,
//     required this.title,
//     required this.value,
//     required this.data,
//     required this.lineColor,
//   });
//
//   @override
//   Widget build(BuildContext context) {
//     return Scaffold(
//       backgroundColor: const Color(0xFFFFF1F2),
//       appBar: AppBar(
//         backgroundColor: const Color(0xFFFFF1F2),
//         elevation: 0,
//         leading: BackButton(color: Colors.black),
//         title: const Text(
//           'VITALTRACK',
//           style: TextStyle(
//             color: Colors.black,
//             letterSpacing: 1.5,
//             fontWeight: FontWeight.bold,
//           ),
//         ),
//         centerTitle: true,
//       ),
//       body: Padding(
//         padding: const EdgeInsets.symmetric(horizontal: 24, vertical: 16),
//         child: Column(
//           crossAxisAlignment: CrossAxisAlignment.start,
//           children: [
//             Text(
//               title,
//               style:
//               const TextStyle(fontSize: 28, fontWeight: FontWeight.bold),
//             ),
//             Text(
//               value,
//               style:
//               const TextStyle(fontSize: 24, fontWeight: FontWeight.w500),
//             ),
//             const SizedBox(height: 16),
//             SizedBox(
//               height: 200,
//               child: LineChart(
//                 LineChartData(
//                   borderData: FlBorderData(show: false),
//                   titlesData: FlTitlesData(show: false),
//                   gridData: FlGridData(show: false),
//                   minY: data.reduce((a, b) => a < b ? a : b) - 5,
//                   maxY: data.reduce((a, b) => a > b ? a : b) + 5,
//                   lineBarsData: [
//                     LineChartBarData(
//                       spots: List.generate(
//                         data.length,
//                             (i) => FlSpot(i.toDouble(), data[i]),
//                       ),
//                       isCurved: true,
//                       color: lineColor,
//                       barWidth: 4,
//                       dotData: FlDotData(show: false),
//                     ),
//                   ],
//                 ),
//               ),
//             ),
//             const SizedBox(height: 24),
//             ToggleButtons(
//               isSelected: const [true, false, false],
//               onPressed: (_) {},
//               borderRadius: BorderRadius.circular(12),
//               selectedColor: Colors.black,
//               fillColor: const Color(0xFFFFE5E7),
//               children: const [
//                 Padding(
//                   padding: EdgeInsets.symmetric(horizontal: 12),
//                   child: Text("Day"),
//                 ),
//                 Padding(
//                   padding: EdgeInsets.symmetric(horizontal: 12),
//                   child: Text("Week"),
//                 ),
//                 Padding(
//                   padding: EdgeInsets.symmetric(horizontal: 12),
//                   child: Text("Month"),
//                 ),
//               ],
//             ),
//             const Spacer(),
//             Center(
//               child: TextButton.icon(
//                 style: TextButton.styleFrom(
//                   backgroundColor: const Color(0xFFFFE5E7),
//                   padding:
//                   const EdgeInsets.symmetric(horizontal: 24, vertical: 12),
//                   shape: RoundedRectangleBorder(
//                     borderRadius: BorderRadius.circular(16),
//                   ),
//                 ),
//                 onPressed: () {},
//                 icon: const Icon(Icons.add, color: Colors.red),
//                 label: const Text(
//                   'Add Measurement',
//                   style: TextStyle(color: Colors.red, fontSize: 16),
//                 ),
//               ),
//             ),
//           ],
//         ),
//       ),
//     );
//   }
// }
//
//

import 'package:flutter/material.dart';
import 'screens/home_screen.dart';

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
      home: const VitalTrackHome(),
      debugShowCheckedModeBanner: false,
    );
  }
}
