import 'package:flutter/material.dart';

class SettingsScreen extends StatelessWidget {
  const SettingsScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: const Color(0xFFFFDADC),
        title: const Text('Settings'),
        centerTitle: true,
      ),
      body: ListView(
        padding: const EdgeInsets.all(24),
        children: [
          _buildSettingCard(
            context,
            icon: Icons.person,
            title: 'Account Settings',
            subtitle: 'Update name, email, password',
            onTap: () {
              // Implement navigation to Account Settings screen
            },
          ),
          _buildSettingCard(
            context,
            icon: Icons.bar_chart,
            title: 'Activity History',
            subtitle: 'View all your health data history',
            onTap: () {
              // Implement navigation to full data history screen
            },
          ),
          _buildSettingCard(
            context,
            icon: Icons.logout,
            title: 'Sign Out',
            subtitle: 'Securely log out of your account',
            onTap: () {
              // Implement sign out logic
              Navigator.popUntil(context, (route) => route.isFirst);
            },
          ),
        ],
      ),
    );
  }

  Widget _buildSettingCard(
      BuildContext context, {
        required IconData icon,
        required String title,
        required String subtitle,
        required VoidCallback onTap,
      }) {
    return Card(
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16)),
      margin: const EdgeInsets.only(bottom: 20),
      child: ListTile(
        contentPadding: const EdgeInsets.all(16),
        leading: Icon(icon, size: 28, color: Colors.black87),
        title: Text(title, style: const TextStyle(fontWeight: FontWeight.bold)),
        subtitle: Text(subtitle),
        trailing: const Icon(Icons.arrow_forward_ios, size: 16),
        onTap: onTap,
      ),
    );
  }
}
