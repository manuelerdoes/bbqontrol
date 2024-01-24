
import { Inter } from "next/font/google";
import "./globals.css";

// components
import Navbar from "./components/Navbar";

const inter = Inter({ subsets: ["latin"] });

export const metadata = {
  title: "BBQONTROL Commander",
  description: "Webinterface to control your BBQONTROL devices",
};

export default function RootLayout({ children }) {
  return (
    <html lang="en">
      <body className={inter.className}>
        <Navbar />
        {children}
      </body>
    </html>
  );
}
