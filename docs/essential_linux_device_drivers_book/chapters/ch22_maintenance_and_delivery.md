# **Chapter 22. Maintenance and Delivery**

| In This Chapter      |     |
|----------------------|-----|
| Coding Style         | 642 |
| Change Markers       | 642 |
| Version Control      | 643 |
| Consistent Checksums | 643 |
| Build Scripts        | 645 |
| Portable Code        | 647 |
|                      |     |

You have reached the end of the device driver tour, but implementing a driver is only a part of the software development life cycle. Before wrapping up, let's discuss a few ideas that contribute to operational efficiency during software maintenance and delivery.

# **Coding Style**

The life span of many Linux devices range from 5 to 10 years, so adherence to a standard coding style helps support the product long after you have moved out of the project.

A powerful editor coupled with an organized writing style makes it easier to correlate code with thought. There can be no infallible guidelines for good style because it's a matter of personal preference, but a uniform manner of coding is invaluable if there are multiple developers working on a project.

Agree on common coding standards with team members and the customer before starting a project. The coding style preferred by kernel developers is described in *Documentation/CodingStyle* in the source tree.
