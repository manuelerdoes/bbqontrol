import Link from "next/link";
import React from 'react'

export default function Navbar() {
  return (
  <nav className="navigation">
    <Link href="/">Dashboard</Link>
    <Link href="/controllers">Controllers</Link>
  </nav>
  )
}
